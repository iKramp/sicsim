#pragma once
#include "backend/machine_state.h"
#include "string"

namespace SicSimBackend {
    void load_program(const std::string &filename, MachineState &machine_state);
}
