#include "instruction.h"
#include "arithmetic.h"
#include "backend/instructions/compare.h"
#include "backend/instructions/device.h"
#include "backend/instructions/jump.h"
#include "backend/instructions/memory.h"
#include "backend/machine_state.h"
#include "backend/number_util.h"
#include "backend/register.h"
#include <stdexcept>

static SicSimBackend::InstructionHandler *instruction_handler_table[256];

namespace SicSimBackend {

    InstructionHandler *get_instruction_handler(uint8_t opcode) {
        // Implementation to return the appropriate instruction handler based on
        // the opcode
        return instruction_handler_table[opcode];
    }

    Format2Instruction decode_format2(uint8_t *instruction,
                                      MachineState &machine_state) {
        Format2Instruction decoded;
        uint8_t reg1 = (instruction[1] >> 4) & 0x0F;
        uint8_t reg2 = instruction[1] & 0x0F;
        decoded.reg1_val = machine_state.getRegisterValue(reg1);
        decoded.reg2_val = machine_state.getRegisterValue(reg2);
        Register reg1_enum = static_cast<Register>(reg1);
        Register reg2_enum = static_cast<Register>(reg2);

        decoded.reg2_target = reg_into_modification_target(reg2_enum);
        //stupid spec, only for clear
        decoded.reg1_target = reg_into_modification_target(reg1_enum);
        return decoded;
    }

    Format34Instruction decode_format34(uint8_t *instruction,
                                       uint24_t instruction_address,
                                       MachineState &machine_state) {
        Format34Instruction decoded;

        bool n = (instruction[0] & 0x02) != 0;
        bool i = (instruction[0] & 0x01) != 0;
        bool x = (instruction[1] & 0x80) != 0;
        bool b = (instruction[1] & 0x40) != 0;
        bool p = (instruction[1] & 0x20) != 0;
        bool e = (instruction[1] & 0x10) != 0;

        //offset is used in:
        //base relative (unsigned, p = 0)
        //pc relative (signed, p = 1)
        //direct (unsigned, p = 0)
        bool offset_signed = p == 0;

        bool x_used = n == i && x;

        uint24_t offset;
        if (e) {
            // Format 4
            offset.set_value(((instruction[1] & 0x0F) << 16) | (instruction[2] << 8) |
                     instruction[3], 20, offset_signed);
        } else {
            // Format 3
            offset.set_value(((instruction[1] & 0x0F) << 8) | instruction[2], 12, offset_signed);
            if (n == 0 && i == 0) { // SIC format
                offset.set_value(((instruction[1] & 0x7F) << 8) | instruction[2], 15, false);
            }
        }

        if (x_used) {
            offset = offset.add(machine_state.rX);
        }

        uint8_t ni;
        uint8_t bp;
        ni = ((uint8_t)n << 1) | (uint8_t)i;
        bp = ((uint8_t)b << 1) | (uint8_t)p;

        uint24_t address;
        uint24_t int_value;
        float48_t float_value;
        bool value_set = false;
        switch (ni) {
        case 0: // SIC
            address = offset;
            break;
        case 1:
            address = offset;
            int_value = offset;
            value_set = true;
            break;
        case 2:
            address = read_memory(offset);
            break;
        case 3:
            switch (bp) {
            case 0:
                address = offset;
                break;
            case 1: //PC relative
                address = instruction_address.add(offset);
                break;
            case 2: //Base relative
                address = machine_state.rB.add(offset);
                break;
            case 3: //Invalid
                throw std::runtime_error("Invalid instruction: both base and pc relative addressing used");
            }
            break;
        }

        if (!value_set) {
            int_value = read_memory(address);
        }
        float_value = read_memory_float(address);

        decoded.value = int_value;
        decoded.float_value = float_value;
        decoded.addr = address;
        return decoded;
    }

    Format2Display display_format2(uint8_t *instruction, MachineState &machine_state) {
        Format2Display display;
        uint8_t reg1 = (instruction[1] >> 4) & 0x0F;
        uint8_t reg2 = instruction[1] & 0x0F;
        display.reg1_name = get_register_name(static_cast<Register>(reg1));
        display.reg2_name = get_register_name(static_cast<Register>(reg2));
        return display;
    }

    Format34Display display_format34(uint8_t *instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Display display;
        display.address_str = "temp";
        return display;
    }

    void init_instruction_handler_table() {
        instruction_handler_table[0x00] = new OpLdReg(Register::A);
        instruction_handler_table[0x01] = nullptr;
        instruction_handler_table[0x02] = nullptr;
        instruction_handler_table[0x03] = nullptr;
        instruction_handler_table[0x04] = new OpLdReg(Register::X);
        instruction_handler_table[0x05] = nullptr;
        instruction_handler_table[0x06] = nullptr;
        instruction_handler_table[0x07] = nullptr;
        instruction_handler_table[0x08] = new OpLdReg(Register::L);
        instruction_handler_table[0x09] = nullptr;
        instruction_handler_table[0x0a] = nullptr;
        instruction_handler_table[0x0b] = nullptr;
        instruction_handler_table[0x0c] = new OpStReg(Register::A);
        instruction_handler_table[0x0d] = nullptr;
        instruction_handler_table[0x0e] = nullptr;
        instruction_handler_table[0x0f] = nullptr;
        instruction_handler_table[0x10] = new OpStReg(Register::X);
        instruction_handler_table[0x11] = nullptr;
        instruction_handler_table[0x12] = nullptr;
        instruction_handler_table[0x13] = nullptr;
        instruction_handler_table[0x14] = new OpStReg(Register::L);
        instruction_handler_table[0x15] = nullptr;
        instruction_handler_table[0x16] = nullptr;
        instruction_handler_table[0x17] = nullptr;
        instruction_handler_table[0x18] = new OpAdd();
        instruction_handler_table[0x19] = new OpAdd();
        instruction_handler_table[0x1a] = new OpAdd();
        instruction_handler_table[0x1b] = new OpAdd();
        instruction_handler_table[0x1c] = new OpSub();
        instruction_handler_table[0x1d] = new OpSub();
        instruction_handler_table[0x1e] = new OpSub();
        instruction_handler_table[0x1f] = new OpSub();
        instruction_handler_table[0x20] = new OpMul();
        instruction_handler_table[0x21] = new OpMul();
        instruction_handler_table[0x22] = new OpMul();
        instruction_handler_table[0x23] = new OpMul();
        instruction_handler_table[0x24] = new OpDiv();
        instruction_handler_table[0x25] = new OpDiv();
        instruction_handler_table[0x26] = new OpDiv();
        instruction_handler_table[0x27] = new OpDiv();
        instruction_handler_table[0x28] = new OpComp();
        instruction_handler_table[0x29] = new OpComp();
        instruction_handler_table[0x2a] = new OpComp();
        instruction_handler_table[0x2b] = new OpComp();
        instruction_handler_table[0x2c] = new OpTix();
        instruction_handler_table[0x2d] = new OpTix();
        instruction_handler_table[0x2e] = new OpTix();
        instruction_handler_table[0x2f] = new OpTix();
        instruction_handler_table[0x30] = new OpConditionalJump(CCState::EQ);
        instruction_handler_table[0x31] = new OpConditionalJump(CCState::EQ);
        instruction_handler_table[0x32] = new OpConditionalJump(CCState::EQ);
        instruction_handler_table[0x33] = new OpConditionalJump(CCState::EQ);
        instruction_handler_table[0x34] = new OpConditionalJump(CCState::GT);
        instruction_handler_table[0x35] = new OpConditionalJump(CCState::GT);
        instruction_handler_table[0x36] = new OpConditionalJump(CCState::GT);
        instruction_handler_table[0x37] = new OpConditionalJump(CCState::GT);
        instruction_handler_table[0x38] = new OpConditionalJump(CCState::LT);
        instruction_handler_table[0x39] = new OpConditionalJump(CCState::LT);
        instruction_handler_table[0x3a] = new OpConditionalJump(CCState::LT);
        instruction_handler_table[0x3b] = new OpConditionalJump(CCState::LT);
        instruction_handler_table[0x3c] = new OpJump();
        instruction_handler_table[0x3d] = new OpJump();
        instruction_handler_table[0x3e] = new OpJump();
        instruction_handler_table[0x3f] = new OpJump();
        instruction_handler_table[0x40] = new OpAnd();
        instruction_handler_table[0x41] = new OpAnd();
        instruction_handler_table[0x42] = new OpAnd();
        instruction_handler_table[0x43] = new OpAnd();
        instruction_handler_table[0x44] = new OpOr();
        instruction_handler_table[0x45] = new OpOr();
        instruction_handler_table[0x46] = new OpOr();
        instruction_handler_table[0x47] = new OpOr();
        instruction_handler_table[0x48] = new OpJSub();
        instruction_handler_table[0x49] = new OpJSub();
        instruction_handler_table[0x4a] = new OpJSub();
        instruction_handler_table[0x4b] = new OpJSub();
        instruction_handler_table[0x4c] = new OpRSub();
        instruction_handler_table[0x4d] = new OpRSub();
        instruction_handler_table[0x4e] = new OpRSub();
        instruction_handler_table[0x4f] = new OpRSub();
        instruction_handler_table[0x50] = new OpLdCh();
        instruction_handler_table[0x51] = new OpLdCh();
        instruction_handler_table[0x52] = new OpLdCh();
        instruction_handler_table[0x53] = new OpLdCh();
        instruction_handler_table[0x54] = new OpStCh();
        instruction_handler_table[0x55] = new OpStCh();
        instruction_handler_table[0x56] = new OpStCh();
        instruction_handler_table[0x57] = new OpStCh();
        instruction_handler_table[0x58] = nullptr;
        instruction_handler_table[0x59] = nullptr;
        instruction_handler_table[0x5a] = nullptr;
        instruction_handler_table[0x5b] = nullptr;
        instruction_handler_table[0x5c] = nullptr;
        instruction_handler_table[0x5d] = nullptr;
        instruction_handler_table[0x5e] = nullptr;
        instruction_handler_table[0x5f] = nullptr;
        instruction_handler_table[0x60] = nullptr;
        instruction_handler_table[0x61] = nullptr;
        instruction_handler_table[0x62] = nullptr;
        instruction_handler_table[0x63] = nullptr;
        instruction_handler_table[0x64] = nullptr;
        instruction_handler_table[0x65] = nullptr;
        instruction_handler_table[0x66] = nullptr;
        instruction_handler_table[0x67] = nullptr;
        instruction_handler_table[0x68] = new OpLdReg(Register::B);
        instruction_handler_table[0x69] = new OpLdReg(Register::B);
        instruction_handler_table[0x6a] = new OpLdReg(Register::B);
        instruction_handler_table[0x6b] = new OpLdReg(Register::B);
        instruction_handler_table[0x6c] = new OpLdReg(Register::S);
        instruction_handler_table[0x6d] = new OpLdReg(Register::S);
        instruction_handler_table[0x6e] = new OpLdReg(Register::S);
        instruction_handler_table[0x6f] = new OpLdReg(Register::S);
        instruction_handler_table[0x70] = new OpLdFlt();
        instruction_handler_table[0x71] = new OpLdFlt();
        instruction_handler_table[0x72] = new OpLdFlt();
        instruction_handler_table[0x73] = new OpLdFlt();
        instruction_handler_table[0x74] = new OpLdReg(Register::T);
        instruction_handler_table[0x75] = new OpLdReg(Register::T);
        instruction_handler_table[0x76] = new OpLdReg(Register::T);
        instruction_handler_table[0x77] = new OpLdReg(Register::T);
        instruction_handler_table[0x78] = new OpStReg(Register::B);
        instruction_handler_table[0x79] = new OpStReg(Register::B);
        instruction_handler_table[0x7a] = new OpStReg(Register::B);
        instruction_handler_table[0x7b] = new OpStReg(Register::B);
        instruction_handler_table[0x7c] = new OpStReg(Register::S);
        instruction_handler_table[0x7d] = new OpStReg(Register::S);
        instruction_handler_table[0x7e] = new OpStReg(Register::S);
        instruction_handler_table[0x7f] = new OpStReg(Register::S);
        instruction_handler_table[0x80] = new OpStFlt();
        instruction_handler_table[0x81] = new OpStFlt();
        instruction_handler_table[0x82] = new OpStFlt();
        instruction_handler_table[0x83] = new OpStFlt();
        instruction_handler_table[0x84] = new OpStReg(Register::T);
        instruction_handler_table[0x85] = new OpStReg(Register::T);
        instruction_handler_table[0x86] = new OpStReg(Register::T);
        instruction_handler_table[0x87] = new OpStReg(Register::T);
        instruction_handler_table[0x88] = nullptr;
        instruction_handler_table[0x89] = nullptr;
        instruction_handler_table[0x8a] = nullptr;
        instruction_handler_table[0x8b] = nullptr;
        instruction_handler_table[0x8c] = nullptr;
        instruction_handler_table[0x8d] = nullptr;
        instruction_handler_table[0x8e] = nullptr;
        instruction_handler_table[0x8f] = nullptr;
        instruction_handler_table[0x90] = new OpAddr();
        instruction_handler_table[0x91] = nullptr;
        instruction_handler_table[0x92] = nullptr;
        instruction_handler_table[0x93] = nullptr;
        instruction_handler_table[0x94] = new OpSubr();
        instruction_handler_table[0x95] = nullptr;
        instruction_handler_table[0x96] = nullptr;
        instruction_handler_table[0x97] = nullptr;
        instruction_handler_table[0x98] = new OpMulr();
        instruction_handler_table[0x99] = nullptr;
        instruction_handler_table[0x9a] = nullptr;
        instruction_handler_table[0x9b] = nullptr;
        instruction_handler_table[0x9c] = new OpDivr();
        instruction_handler_table[0x9d] = nullptr;
        instruction_handler_table[0x9e] = nullptr;
        instruction_handler_table[0x9f] = nullptr;
        instruction_handler_table[0xa0] = new OpCompr();
        instruction_handler_table[0xa1] = nullptr;
        instruction_handler_table[0xa2] = nullptr;
        instruction_handler_table[0xa3] = nullptr;
        instruction_handler_table[0xa4] = new OpShiftL();
        instruction_handler_table[0xa5] = nullptr;
        instruction_handler_table[0xa6] = nullptr;
        instruction_handler_table[0xa7] = nullptr;
        instruction_handler_table[0xa8] = new OpShiftR();
        instruction_handler_table[0xa9] = nullptr;
        instruction_handler_table[0xaa] = nullptr;
        instruction_handler_table[0xab] = nullptr;
        instruction_handler_table[0xac] = new OpRmo();
        instruction_handler_table[0xad] = nullptr;
        instruction_handler_table[0xae] = nullptr;
        instruction_handler_table[0xaf] = nullptr;
        instruction_handler_table[0xb0] = nullptr;
        instruction_handler_table[0xb1] = nullptr;
        instruction_handler_table[0xb2] = nullptr;
        instruction_handler_table[0xb3] = nullptr;
        instruction_handler_table[0xb4] = new OpClear();
        instruction_handler_table[0xb5] = nullptr;
        instruction_handler_table[0xb6] = nullptr;
        instruction_handler_table[0xb7] = nullptr;
        instruction_handler_table[0xb8] = new OpTixr();
        instruction_handler_table[0xb9] = nullptr;
        instruction_handler_table[0xba] = nullptr;
        instruction_handler_table[0xbb] = nullptr;
        instruction_handler_table[0xbc] = nullptr;
        instruction_handler_table[0xbd] = nullptr;
        instruction_handler_table[0xbe] = nullptr;
        instruction_handler_table[0xbf] = nullptr;
        instruction_handler_table[0xc0] = nullptr;
        instruction_handler_table[0xc1] = nullptr;
        instruction_handler_table[0xc2] = nullptr;
        instruction_handler_table[0xc3] = nullptr;
        instruction_handler_table[0xc4] = nullptr;
        instruction_handler_table[0xc5] = nullptr;
        instruction_handler_table[0xc6] = nullptr;
        instruction_handler_table[0xc7] = nullptr;
        instruction_handler_table[0xc8] = nullptr;
        instruction_handler_table[0xc9] = nullptr;
        instruction_handler_table[0xca] = nullptr;
        instruction_handler_table[0xcb] = nullptr;
        instruction_handler_table[0xcc] = nullptr;
        instruction_handler_table[0xcd] = nullptr;
        instruction_handler_table[0xce] = nullptr;
        instruction_handler_table[0xcf] = nullptr;
        instruction_handler_table[0xd0] = new OpLdReg(Register::SW);
        instruction_handler_table[0xd1] = new OpLdReg(Register::SW);
        instruction_handler_table[0xd2] = new OpLdReg(Register::SW);
        instruction_handler_table[0xd3] = new OpLdReg(Register::SW);
        instruction_handler_table[0xd4] = nullptr;
        instruction_handler_table[0xd5] = nullptr;
        instruction_handler_table[0xd6] = nullptr;
        instruction_handler_table[0xd7] = nullptr;
        instruction_handler_table[0xd8] = new OpReadDevice();
        instruction_handler_table[0xd9] = new OpReadDevice();
        instruction_handler_table[0xda] = new OpReadDevice();
        instruction_handler_table[0xdb] = new OpReadDevice();
        instruction_handler_table[0xdc] = new OpWriteDevice();
        instruction_handler_table[0xdd] = new OpWriteDevice();
        instruction_handler_table[0xde] = new OpWriteDevice();
        instruction_handler_table[0xdf] = new OpWriteDevice();
        instruction_handler_table[0xe0] = new OpTestDevice();
        instruction_handler_table[0xe1] = new OpTestDevice();
        instruction_handler_table[0xe2] = new OpTestDevice();
        instruction_handler_table[0xe3] = new OpTestDevice();
        instruction_handler_table[0xe4] = nullptr;
        instruction_handler_table[0xe5] = nullptr;
        instruction_handler_table[0xe6] = nullptr;
        instruction_handler_table[0xe7] = nullptr;
        instruction_handler_table[0xe8] = new OpStReg(Register::SW);
        instruction_handler_table[0xe9] = new OpStReg(Register::SW);
        instruction_handler_table[0xea] = new OpStReg(Register::SW);
        instruction_handler_table[0xeb] = new OpStReg(Register::SW);
        instruction_handler_table[0xec] = nullptr;
        instruction_handler_table[0xed] = nullptr;
        instruction_handler_table[0xee] = nullptr;
        instruction_handler_table[0xef] = nullptr;
        instruction_handler_table[0xf0] = nullptr;
        instruction_handler_table[0xf1] = nullptr;
        instruction_handler_table[0xf2] = nullptr;
        instruction_handler_table[0xf3] = nullptr;
        instruction_handler_table[0xf4] = nullptr;
        instruction_handler_table[0xf5] = nullptr;
        instruction_handler_table[0xf6] = nullptr;
        instruction_handler_table[0xf7] = nullptr;
        instruction_handler_table[0xf8] = nullptr;
        instruction_handler_table[0xf9] = nullptr;
        instruction_handler_table[0xfa] = nullptr;
        instruction_handler_table[0xfb] = nullptr;
        instruction_handler_table[0xfc] = nullptr;
        instruction_handler_table[0xfd] = nullptr;
        instruction_handler_table[0xfe] = nullptr;
        instruction_handler_table[0xff] = nullptr;
    }
} // namespace SicSimBackend
