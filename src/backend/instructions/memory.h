#pragma once
#include "backend/register.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpLdReg : public InstructionHandler {
        Register reg;
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpLdReg(Register reg) : reg(reg) {}
    };

    class OpStReg : public InstructionHandler {
        Register reg;
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpStReg(Register reg) : reg(reg) {}
    };

    class OpLdCh : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpStCh : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpLdFlt : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpStFlt : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
