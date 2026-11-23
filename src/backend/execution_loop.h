#pragma once
#include "machine_state.h"

namespace SicSimBackend {
    enum class StepResult {
        CONTINUE,
        HALT,
        INVALID_INSTRUCTION,
    };

    void init_execution_loop();
    StepResult step_sim();
    MachineState &get_machine_state();
} // namespace ExecutionLoop
