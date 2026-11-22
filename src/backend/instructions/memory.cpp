#include "memory.h"
#include "backend/register.h"

namespace SicSimBackend {
    ModificationList OpLdReg::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(reg_into_modification_target(reg), 0),
                                               decoded.value.get_unsigned()));
        return modifications;
    }
    std::string OpLdReg::display_instruction(uint32_t instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        std::string out =
            "LD" + get_register_name(reg) + ", " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpStReg::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM3B, decoded.addr),
                                               machine_state.get_register_value(static_cast<uint8_t>(reg)).get_unsigned()));
        return modifications;
    }
    std::string OpStReg::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "ST" + get_register_name(reg) + ", " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpLdCh::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM1B, decoded.value),
                                               machine_state.rA.get_unsigned() & 0xFF));
        return modifications;
    }
    std::string OpLdCh::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "LDCH " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpStCh::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM1B, decoded.addr),
                                               machine_state.rA.get_unsigned() & 0xFF));
        return modifications;
    }
    std::string OpStCh::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "STCH " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpLdFlt::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM6B, decoded.value),
                                               machine_state.rA.get_unsigned() & 0xFFFFFFFFFFFF));
        return modifications;
    }
    std::string OpLdFlt::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "LDF " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpStFlt::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM6B, decoded.addr),
                                               machine_state.rA.get_unsigned() & 0xFFFFFFFFFFFF));
        return modifications;
    }
    std::string OpStFlt::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "STF " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
} // namespace SicSimBackend
