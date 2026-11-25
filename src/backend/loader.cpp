#include "loader.h"
#include "backend/machine_state.h"
#include <fstream>
#include <iostream>
#include <string>

namespace SicSimBackend {
    bool load_program(const std::string &filename, MachineState &machine_state) {
        machine_state = MachineState(); // Reset the machine state
        std::ifstream file(filename, std::ios::binary);

        if (!file) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::vector<uint8_t> program_bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        return load_program_bytes(program_bytes, machine_state);
    }

    bool load_program_bytes(const std::vector<uint8_t> &program_bytes, MachineState &machine_state) {
        uint32_t loaded_bytes = 0;
        while (1) {
            if (loaded_bytes >= program_bytes.size()) {
                break;
            }

            int newline_index = program_bytes.size();
            for (uint32_t i = loaded_bytes; i < program_bytes.size(); i++) {
                if (program_bytes[i] == '\n') {
                    newline_index = i;
                    break;
                }
            }
            std::string line;
            line = std::string(program_bytes.begin() + loaded_bytes, program_bytes.begin() + newline_index);
            loaded_bytes = newline_index + 1;
            if (line.empty()) {
                continue;
            }
            unsigned char record_type = line[0];

            switch (record_type) {
            case 'H': {
                std::cout << "Loading program " << line.substr(1, 6) << std::endl;
                break;
            }
            case 'T': {
                uint32_t start_address = std::stoul(line.substr(1, 6), nullptr, 16);
                uint32_t length = std::stoul(line.substr(7, 2), nullptr, 16);

                for (uint32_t i = 0; i < length; ++i) {
                    uint8_t byte = std::stoul(line.substr(9 + i * 2, 2), nullptr, 16);
                    write_memory_byte(start_address + i, byte);
                }
                break;
            }
            case 'E': {
                uint32_t start_address = std::stoul(line.substr(1, 6), nullptr, 16);
                machine_state.PC = uint24_t(start_address);

                std::cout << "Setting program counter to 0x" << std::hex << start_address << std::dec << std::endl;
                break;
            }
            case 'M': {
                // Modification record, can be ignored for loading purposes
                break;
            }
            case 'D': {
                // Define record, can be ignored for loading purposes
                break;
            }
            case 'R': {
                // Reference record, can be ignored for loading purposes
                break;
            }
            default: {
                // Invalid record type, can be ignored for loading purposes
                break;
            }
            }
        }
        return true;
    }
} // namespace SicSimBackend
