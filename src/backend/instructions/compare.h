#pragma once
#include "backend/machine_state.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpComp : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpCompr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpTix : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpTixr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
