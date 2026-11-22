#pragma once
#include "backend/machine_state.h"
#include <cstdint>

namespace SicSimBackend {
    class InstructionHandler {
    public:
        virtual ModificationList handle(uint32_t instruction,
                            uint24_t instruction_address, MachineState &machine_state) = 0;
        virtual std::string display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) = 0;
    };

    class Format2Instruction {
    public:
        uint8_t reg1;
        uint8_t reg2;
        ModificationTarget reg2_target;
        ModificationTarget reg1_target;
        uint24_t reg1_val;
        uint24_t reg2_val;
    };

    class Format2Display {
    public:
        std::string reg1_name;
        std::string reg2_name;
    };

    class Format34Instruction {
    public:
        uint24_t addr;
        uint24_t value;
        float48_t float_value;
    };

    class Format34Display {
    public:
        std::string address_str;
    };

    void init_instruction_handler_table();
    InstructionHandler *get_instruction_handler(uint8_t opcode);

    Format2Instruction decode_format2(uint8_t *instruction, MachineState &machine_state);
    Format34Instruction decode_format34(uint8_t *instruction, uint24_t instruction_address, MachineState &machine_state);

    Format2Display display_format2(uint8_t *instruction, MachineState &machine_state);
    Format34Display display_format34(uint8_t *instruction, uint24_t instruction_address, MachineState &machine_state);
} // namespace SicSimBackend
