#pragma once
#include "machine_state.h"

namespace SicSimBackend {
    enum class StepResult {
        CONTINUE,
        HALT,
        INVALID_INSTRUCTION,
    };

    void init_execution_loop();
    StepResult single_step();
    StepResult step_sim(uint32_t n_steps, uint32_t step_to_addr);
    MachineState &get_machine_state();
} // namespace ExecutionLoop
