#include "jump.h"
#include <array>

namespace SicSimBackend {
    ModificationList OpJump::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;

        bool halt = instruction_address.get_unsigned() == decoded.addr.get_unsigned();
        if (halt) {
            modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::HALT, 0), 0));
        } else {
            modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), decoded.addr.get_unsigned()));
        }
        return modifications;
    }
    std::string OpJump::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "J " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpJump::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpConditionalJump::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        uint8_t cc_state = (machine_state.rSW.get_unsigned() >> 16) & 0x03;
        if (cc_state == static_cast<uint8_t>(condition)) {
            modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), decoded.addr.get_unsigned()));
        }
        return modifications;
    }
    std::string OpConditionalJump::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string cond = condition == CCState::EQ ? "EQ" :
            (condition == CCState::GT ? "GT" : "LT");
        std::string out = "J" + cond + " " +
                          display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpConditionalJump::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpJSub::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), decoded.addr.get_unsigned()));
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::L, 0), instruction_address.add(3).get_unsigned()));
        return modifications;
    }
    std::string OpJSub::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "JSUB " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpJSub::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpRSub::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), machine_state.rL.get_unsigned()));
        return modifications;
    }
    std::string OpRSub::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "RSUB";
        return out;
    }
    uint8_t OpRSub::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        return 1;
    }
} // namespace SicSimBackend
