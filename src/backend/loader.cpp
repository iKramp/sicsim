#include "loader.h"
#include "backend/machine_state.h"
#include <fstream>
#include <iostream>
#include <string>

namespace SicSimBackend {
    void load_program(const std::string &filename, MachineState &machine_state) {
        machine_state = MachineState(); // Reset the machine state
        std::ifstream program;
        program.open(filename);

        while (!program.eof()) {
            std::string line;
            std::getline(program, line);
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

                std::cout << "Setting program counter to 0x" << std::hex << start_address << std::endl;
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
    }
} // namespace SicSimBackend
