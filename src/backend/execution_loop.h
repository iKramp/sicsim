#pragma once
#include "machine_state.h"

namespace SicSimBackend {
    enum class StepResult {
        CONTINUE,
        HALT,
        INVALID_INSTRUCTION,
    };

    void init_execution_loop();
    void load_program(const std::vector<uint8_t> &program, uint32_t start_address);
    StepResult step_sim();
    MachineState &get_machine_state();
} // namespace ExecutionLoop
