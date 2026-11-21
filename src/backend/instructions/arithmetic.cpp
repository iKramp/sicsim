#include "arithmetic.h"
#include "backend/machine_state.h"
#include <utility>

namespace SicSimBackend {
    ModificationList OpAdd::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               decoded.value.add(machine_state.rA).get_unsigned()));
        return modifications;
    }
    std::string OpAdd::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                           MachineState &machine_state) {
        return "ADD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state)
                            .address_str;
    }

    ModificationList OpAddr::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.add(decoded.reg2_val).get_unsigned()));
        return modifications;
    }
    std::string OpAddr::display_instruction(uint32_t instruction, uint32_t _instruction_address,
                                            MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "ADDR " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpAnd::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               decoded.value.bit_and(machine_state.rA).get_unsigned()));
        return modifications;
    }
    std::string OpAnd::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                           MachineState &machine_state) {
        return "AND " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state)
                            .address_str;
    }

    ModificationList OpClear::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        // stupid spec puts target in reg2, but for 1 reg instructions the target is reg1?????
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(decoded.reg1_target, 0), 0));
        return modifications;
    }
    std::string OpClear::display_instruction(uint32_t instruction, uint32_t _instruction_address,
                                             MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "CLEAR " + display.reg1_name;
    }

    ModificationList OpDiv::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               machine_state.rA.div(decoded.value).get_unsigned()));
        return modifications;
    }
    std::string OpDiv::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                           MachineState &machine_state) {
        return "DIV " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
    }

    ModificationList OpDivr::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.div(decoded.reg2_val).get_unsigned()));
        return modifications;
    }
    std::string OpDivr::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "DIVR " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpMul::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               decoded.value.mul(machine_state.rA).get_unsigned()));
        return modifications;
    }
    std::string OpMul::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                           MachineState &machine_state) {
        return "MUL " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
    }

    ModificationList OpMulr::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.mul(decoded.reg2_val).get_unsigned()));
        return modifications;
    }
    std::string OpMulr::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "MULR " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpOr::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               decoded.value.bit_or(machine_state.rA).get_unsigned()));
        return modifications;
    }
    std::string OpOr::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                          MachineState &machine_state) {
        return "OR " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
    }

    ModificationList OpRmo::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.get_unsigned()));
        return modifications;
    }
    std::string OpRmo::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "RMO " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpShiftL::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.shl(decoded.reg2 + 1).get_unsigned()));
        return modifications;
    }
    std::string OpShiftL::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "SHIFTL " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpShiftR::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.shr(decoded.reg2 + 1, true).get_unsigned()));
        return modifications;
    }
    std::string OpShiftR::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "SHIFTR " + display.reg1_name + ", " + display.reg2_name;
    }

    ModificationList OpSub::handle(uint32_t instruction, uint32_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded =
            decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0),
                                               machine_state.rA.sub(decoded.value).get_unsigned()));
        return modifications;
    }
    std::string OpSub::display_instruction(uint32_t instruction, uint32_t instruction_address,
                                           MachineState &machine_state) {
        return "SUB " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
    }

    ModificationList OpSubr::handle(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Instruction decoded = decode_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        ModificationList modifications;
        modifications.push_back(
            std::make_pair(std::make_pair(decoded.reg2_target, 0), decoded.reg1_val.sub(decoded.reg2_val).get_unsigned()));
        return modifications;
    }
    std::string OpSubr::display_instruction(uint32_t instruction, uint32_t _instruction_address, MachineState &machine_state) {
        Format2Display display = display_format2(reinterpret_cast<uint8_t *>(&instruction), machine_state);
        return "SUBR " + display.reg1_name + ", " + display.reg2_name;
    }

} // namespace SicSimBackend
