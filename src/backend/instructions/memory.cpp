#include "memory.h"
#include <array>
#include "backend/instructions/instruction.h"
#include "backend/register.h"

namespace SicSimBackend {
    ModificationList OpLdReg::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(reg_into_modification_target(reg), 0),
                                               decoded.value.get_unsigned()));
        return modifications;
    }
    std::string OpLdReg::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        std::string out =
            "LD" + get_register_name(reg) + ", " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpLdReg::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpStReg::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM3B, decoded.addr),
                                               machine_state.get_register_value(static_cast<uint8_t>(reg)).get_unsigned()));
        return modifications;
    }
    std::string OpStReg::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "ST" + get_register_name(reg) + ", " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpStReg::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpLdCh::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM1B, decoded.value),
                                               machine_state.rA.get_unsigned() & 0xFF));
        return modifications;
    }
    std::string OpLdCh::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "LDCH " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpLdCh::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpStCh::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM1B, decoded.addr),
                                               machine_state.rA.get_unsigned() & 0xFF));
        return modifications;
    }
    std::string OpStCh::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "STCH " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpStCh::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpLdFlt::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM6B, decoded.value),
                                               machine_state.rA.get_unsigned() & 0xFFFFFFFFFFFF));
        return modifications;
    }
    std::string OpLdFlt::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "LDF " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpLdFlt::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpStFlt::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::MEM6B, decoded.addr),
                                               machine_state.rA.get_unsigned() & 0xFFFFFFFFFFFF));
        return modifications;
    }
    std::string OpStFlt::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out =
            "STF " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpStFlt::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }
} // namespace SicSimBackend
