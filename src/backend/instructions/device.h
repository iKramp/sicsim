#pragma once
#include "backend/machine_state.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpReadDevice : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpWriteDevice : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpTestDevice : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
