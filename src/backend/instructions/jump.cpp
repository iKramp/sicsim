#include "jump.h"

namespace SicSimBackend {
    ModificationList OpJump::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
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
    std::string OpJump::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "J " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpConditionalJump::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        uint8_t cc_state = (machine_state.rSW.get_unsigned() >> 16) & 0x03;
        if (cc_state == static_cast<uint8_t>(condition)) {
            modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), decoded.addr.get_unsigned()));
        }
        return modifications;
    }
    std::string OpConditionalJump::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "J" + std::to_string(static_cast<uint8_t>(condition)) + " " +
                          display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpJSub::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), decoded.addr.get_unsigned()));
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::L, 0), instruction_address.add(3).get_unsigned()));
        return modifications;
    }
    std::string OpJSub::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "JSUB " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpRSub::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::PC, 0), machine_state.rL.get_unsigned()));
        return modifications;
    }
    std::string OpRSub::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "RSUB";
        return out;
    }
} // namespace SicSimBackend
