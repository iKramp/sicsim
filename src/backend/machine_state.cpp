#include "machine_state.h"
#include <cstring>
#include <stdexcept>

#define MEM_SIZE (1 << 20)

static uint8_t memory[MEM_SIZE];
static uint32_t max_written_address = 0;

namespace SicSimBackend {
    MachineState::MachineState()
        : rA(uint24_t()), rX(uint24_t()), rL(uint24_t()), rS(uint24_t()), rT(uint24_t()), rB(uint24_t()), rSW(uint24_t()), rF(0), PC(uint24_t()) {}

    MachineState MachineState::clone() {
        MachineState copy;
        copy.rA = rA;
        copy.rX = rX;
        copy.rL = rL;
        copy.rS = rS;
        copy.rT = rT;
        copy.rB = rB;
        copy.rSW = rSW;
        copy.rF = rF;
        copy.PC = PC;
        return copy;
    }

    void MachineState::apply_modifications(ModificationList modifications) {
        bool pc_modified = false;

        for (const auto &modification : modifications) {
            switch (modification.first.first) {
            case ModificationTarget::A:
                rA.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::AL:
                rA.set_value((rA.get_unsigned() & 0xFFFF00) | (modification.second & 0xFF), 24, false);
                break;
            case ModificationTarget::X:
                rX.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::L:
                rL.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::S:
                rS.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::T:
                rT.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::B:
                rB.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::SW:
                rSW.set_value(modification.second, 24, false);
                break;
            case ModificationTarget::F:
                throw std::runtime_error("ModificationTarget::F is not supported yet");
                break;
            case ModificationTarget::PC:
                PC.set_value(modification.second, 24, false);
                pc_modified = true;
                break;
            case ModificationTarget::MEM1B:
                // write_memory(modification.second, modification.second, 1);
                write_memory_byte(modification.first.second, static_cast<uint8_t>(modification.second & 0xFF));
                break;
            case ModificationTarget::MEM3B:
                write_memory(modification.first.second, modification.second);
                break;
            case ModificationTarget::MEM6B:
                write_memory_float(modification.first.second, modification.second);
                break;
            case ModificationTarget::HALT:
                break;
            }
        }

        if (!pc_modified) {
            PC = PC.add(3);
        }
    }

    uint24_t MachineState::get_register_value(uint8_t reg_num) {
        switch (reg_num) {
        case 0:
            return rA;
        case 1:
            return rX;
        case 2:
            return rL;
        case 3:
            return rB;
        case 4:
            return rS;
        case 5:
            return rT;
        case 8:
            return PC;
        case 9:
            return rSW;
        default:
            return 0; // Invalid register number
        }
    }

    void MachineState::print_state() {
        printf("A: 0x%06X, X: 0x%06X, L: 0x%06X, S: 0x%06X, T: 0x%06X, B: 0x%06X, SW: 0x%06X, F: 0x%012lX, PC: 0x%06X\n", rA.get_unsigned(),
               rX.get_unsigned(), rL.get_unsigned(), rS.get_unsigned(), rT.get_unsigned(), rB.get_unsigned(), rSW.get_unsigned(), rF,
               PC.get_unsigned());
    }

    void init_memory() {
        memset(memory, 0, MEM_SIZE);
        max_written_address = 0;
    }

    void write_memory_region(uint24_t address, const std::vector<uint8_t> &data) {
        uint32_t addr = address.get_unsigned();
        if (addr + data.size() > MEM_SIZE) {
            return;
        }
        memcpy(&memory[addr], data.data(), data.size());
        max_written_address = std::max(max_written_address, addr + static_cast<uint32_t>(data.size()));
    }

    void write_memory_byte(uint24_t address, uint8_t data) {
        if (address.get_unsigned() >= MEM_SIZE) {
            return;
        }
        memory[address.get_unsigned()] = data;
        max_written_address = std::max(max_written_address, address.get_unsigned() + 1);
    }

    void write_memory(uint24_t address, uint24_t value) {
        if (address.get_unsigned() + 3 > MEM_SIZE) {
            return;
        }
        memory[address.get_unsigned()] = static_cast<uint8_t>((value.get_unsigned() >> 16) & 0xFF);
        memory[address.get_unsigned() + 1] = static_cast<uint8_t>((value.get_unsigned() >> 8) & 0xFF);
        memory[address.get_unsigned() + 2] = static_cast<uint8_t>(value.get_unsigned() & 0xFF);
        max_written_address = std::max(max_written_address, address.get_unsigned() + 3);
    }

    void write_memory_float(uint24_t address, float48_t value) {
        if (address.get_unsigned() + 6 > MEM_SIZE) {
            return;
        }
        throw std::runtime_error("write_memory_float is not implemented yet");
        max_written_address = std::max(max_written_address, address.get_unsigned() + 6);
    }

    uint8_t read_memory_byte(uint24_t address) {
        if (address.get_unsigned() >= MEM_SIZE) {
            return 0;
        }
        return memory[address.get_unsigned()];
    }

    uint24_t read_memory(uint24_t address) {
        if (address.get_unsigned() + 3 > MEM_SIZE) {
            return 0;
        }
        uint32_t value = (static_cast<uint32_t>(memory[address.get_unsigned()]) << 16) |
                         (static_cast<uint32_t>(memory[address.get_unsigned() + 1]) << 8) | static_cast<uint32_t>(memory[address.get_unsigned() + 2]);
        return uint24_t(value);
    }

    std::pair<uint32_t, bool> read_memory_instruction(uint24_t address) {
        if (address.get_unsigned() + 3 > MEM_SIZE) {
            return std::make_pair(0, false);
        }

        bool valid_instruction = address.get_unsigned() <= max_written_address;

        uint32_t instruction = (static_cast<uint32_t>(memory[address.get_unsigned()]) << 16) |
                               (static_cast<uint32_t>(memory[address.get_unsigned() + 1]) << 8) |
                               static_cast<uint32_t>(memory[address.get_unsigned() + 2]);
        return std::make_pair(instruction, valid_instruction);
    }

    float48_t read_memory_float(uint24_t address) {
        if (address.get_unsigned() + 6 > MEM_SIZE) {
            return float48_t(0);
        }
        throw std::runtime_error("read_memory_float is not implemented yet");
    }

    uint8_t *get_memory_pointer(uint32_t address) {
        if (address >= MEM_SIZE) {
            return nullptr;
        }
        return &memory[address];
    }

    ModificationTarget reg_into_modification_target(Register reg) {
        switch (reg) {
        case Register::A:
            return ModificationTarget::A;
        case Register::X:
            return ModificationTarget::X;
        case Register::L:
            return ModificationTarget::L;
        case Register::S:
            return ModificationTarget::S;
        case Register::T:
            return ModificationTarget::T;
        case Register::B:
            return ModificationTarget::B;
        case Register::SW:
            return ModificationTarget::SW;
        case Register::PC:
            return ModificationTarget::PC;
        case Register::F:
            return ModificationTarget::F;
        }
    }

} // namespace SicSimBackend
