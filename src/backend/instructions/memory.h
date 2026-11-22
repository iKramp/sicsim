#pragma once
#include "backend/register.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpLdReg : public InstructionHandler {
        Register reg;
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpLdReg(Register reg) : reg(reg) {}
    };

    class OpStReg : public InstructionHandler {
        Register reg;
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpStReg(Register reg) : reg(reg) {}
    };

    class OpLdCh : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpStCh : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpLdFlt : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpStFlt : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
