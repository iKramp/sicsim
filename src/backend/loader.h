#pragma once
#include "backend/machine_state.h"
#include "string"

namespace SicSimBackend {
    bool load_program(const std::string &filename, MachineState &machine_state);
    bool load_program_bytes(const std::vector<uint8_t> &program_bytes, MachineState &machine_state);
}
