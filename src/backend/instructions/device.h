#pragma once
#include "backend/machine_state.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpReadDevice : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpWriteDevice : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpTestDevice : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
