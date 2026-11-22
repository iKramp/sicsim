#pragma once
#include "backend/machine_state.h"

namespace SicSimUi {
    int init_ui();
    void update_ui(SicSimBackend::MachineState &machine_state);
    void cleanup_ui();
    bool should_close_ui();
}
