#include "compare.h"
#include <array>

namespace SicSimBackend {
    ModificationList OpComp::handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        uint24_t regA_val = machine_state.rA;

        uint8_t cc_state;
        if (regA_val.get_signed() < decoded.value.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::LT);
        } else if (regA_val.get_signed() > decoded.value.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::GT);
        } else {
            cc_state = static_cast<uint8_t>(CCState::EQ);
        }
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (cc_state << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpComp::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        std::string out =
            "COMP " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpComp::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpCompr::handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) {
        Format2Instruction decoded =
            decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        uint24_t reg1_val = decoded.reg1_val;
        uint24_t reg2_val = decoded.reg2_val;

        uint8_t cc_state;
        if (reg1_val.get_signed() < reg2_val.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::LT);
        } else if (reg1_val.get_signed() > reg2_val.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::GT);
        } else {
            cc_state = static_cast<uint8_t>(CCState::EQ);
        }
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (cc_state << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpCompr::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        Format2Display out = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        std::string out_str = "COMPR " + out.reg1_name + ", " + out.reg2_name;
        return out_str;
    }
    uint8_t OpCompr::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        return 2;
    }

    ModificationList OpTix::handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        uint24_t regX_val = machine_state.rX;

        uint24_t newX_val = regX_val.add(1);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::X, 0), newX_val.get_unsigned()));

        uint8_t cc_state;
        if (newX_val.get_signed() < decoded.value.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::LT);
        } else if (newX_val.get_signed() > decoded.value.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::GT);
        } else {
            cc_state = static_cast<uint8_t>(CCState::EQ);
        }
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (cc_state << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpTix::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        std::string out =
            "TIX " +
            display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpTix::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpTixr::handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) {
        Format2Instruction decoded =
            decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        uint24_t regX_val = machine_state.rX;
        uint24_t newX_val = regX_val.add(1);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::X, 0), newX_val.get_unsigned()));

        uint24_t reg1_val = decoded.reg1_val;

        uint8_t cc_state;
        if (newX_val.get_signed() < reg1_val.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::LT);
        } else if (newX_val.get_signed() > reg1_val.get_signed()) {
            cc_state = static_cast<uint8_t>(CCState::GT);
        } else {
            cc_state = static_cast<uint8_t>(CCState::EQ);
        }
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (cc_state << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpTixr::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address,
                                           MachineState &machine_state) {
        Format2Display out = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        std::string out_str = "TIXR " + out.reg1_name;
        return out_str;
    }
    uint8_t OpTixr::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        return 2;
    }
}
