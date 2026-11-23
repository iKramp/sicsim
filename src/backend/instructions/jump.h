#pragma once
#include "backend/register.h"
#include "instruction.h"

namespace SicSimBackend {
    class OpJump : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpConditionalJump : public InstructionHandler {
        CCState condition;
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        public:
        OpConditionalJump(CCState condition) : condition(condition) {}
    };

    class OpJSub : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };

    class OpRSub : public InstructionHandler {
        ModificationList handle(std::array<uint8_t, 4> instruction,
                            uint24_t instruction_address, MachineState &machine_state) override;
        std::string display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
        uint8_t n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) override;
    };
}
