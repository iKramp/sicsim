#pragma once

#include "backend/register.h"
#include "number_util.h"
#include <cstdint>
#include <string>
#include <vector>

namespace SicSimBackend {
    enum class ModificationTarget {
        A,
        AL,
        X,
        L,
        S,
        T,
        B,
        SW,
        F,
        PC,
        MEM1B,
        MEM3B,
        MEM6B,
        HALT,
    };

    typedef std::vector<std::pair<std::pair<ModificationTarget, uint24_t>, uint64_t>> ModificationList;

    class MachineState {
    public:
        uint24_t rA;
        uint24_t rX;
        uint24_t rL;
        uint24_t rS;
        uint24_t rT;
        uint24_t rB;
        uint24_t rSW;
        uint64_t rF;

        // not yet executed
        uint24_t PC;

        MachineState();

        std::string displayCurrentInstruction();
        MachineState clone();
        // returns what the next execution will modify
        void applyModifications(ModificationList modifications);

        uint24_t getRegisterValue(uint8_t reg_num);
    };

    void init_memory();
    void write_memory_region(uint24_t address, const std::vector<uint8_t> &data);
    void write_memory(uint24_t address, uint24_t value);
    void write_memory_float(uint24_t address, float48_t value);
    void write_memory_byte(uint24_t address, uint8_t data);
    uint24_t read_memory(uint24_t address);
    float48_t read_memory_float(uint24_t address);
    //returns if there's any valid memory after this instruction
    std::pair<uint32_t, bool> read_memory_instruction(uint24_t address);
    uint8_t read_memory_byte(uint24_t address);
    uint8_t *get_memory_pointer(uint24_t address);
    ModificationTarget reg_into_modification_target(Register reg);

} // namespace SicSimBackend
