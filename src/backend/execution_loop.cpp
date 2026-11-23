#include "execution_loop.h"
#include <array>
#include "backend/instructions/instruction.h"
#include "backend/machine_state.h"

static SicSimBackend::MachineState machine_state;

namespace SicSimBackend {
    void init_execution_loop() {
        machine_state = MachineState();
        init_memory();
    }

    StepResult step_sim() {
        uint32_t instruction_address = machine_state.PC.get_unsigned();
        std::pair<std::array<uint8_t, 4>, bool> instruction = read_memory_instruction(instruction_address);

        if (!instruction.second) {
            return StepResult::HALT;
        }

        InstructionHandler *handler = get_instruction_handler(instruction.first[0]);
        if (handler == nullptr) {
            return StepResult::INVALID_INSTRUCTION;
        }
        ModificationList modifications = handler->handle(instruction.first, instruction_address, machine_state);
        uint8_t instruction_length = handler->n_bytes(instruction.first, instruction_address, machine_state);
        modifications.push_back({{ModificationTarget::PC, 0}, instruction_address + instruction_length});
        machine_state.apply_modifications(modifications);

        for (const auto &modification : modifications) {
            if (modification.first.first == ModificationTarget::HALT) {
                return StepResult::HALT;
            }
        }
        return StepResult::CONTINUE;
    }

    MachineState &get_machine_state() {
        return machine_state;
    }
} // namespace SicSimBackend
