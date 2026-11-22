#pragma once
#include "instruction.h"

namespace SicSimBackend {
    class OpAdd : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpAddr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpAnd : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpClear : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpDiv : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpDivr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpMul : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpMulr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpOr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpRmo : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpShiftL : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpShiftR : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpSub : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
    class OpSubr : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
} // namespace SicSimBackend
