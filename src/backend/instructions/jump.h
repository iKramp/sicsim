#pragma once
#include "backend/register.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpJump : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpConditionalJump : public InstructionHandler {
        CCState condition;
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpConditionalJump(CCState condition) : condition(condition) {}
    };

    class OpJSub : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpRSub : public InstructionHandler {
        ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
