#include "execution_loop.h"
#include "backend/instructions/instruction.h"
#include "backend/machine_state.h"

static SicSimBackend::MachineState machine_state;

namespace SicSimBackend {
    void init_execution_loop() {
        machine_state = MachineState();
        init_memory();
    }

    void load_program(const std::vector<uint8_t> &program, uint32_t start_address) {
        write_memory_region(start_address, program);
        machine_state.PC = start_address;
    }

    StepResult step_sim() {
        uint32_t instruction_address = machine_state.PC.get_unsigned();
        std::pair<uint32_t, bool> instruction = read_memory_instruction(instruction_address);

        if (!instruction.second) {
            return StepResult::HALT;
        }

        InstructionHandler *handler = get_instruction_handler(instruction.first & 0xFF);
        if (handler == nullptr) {
            return StepResult::INVALID_INSTRUCTION;
        }
        ModificationList modifications = handler->handle(instruction.first, instruction_address, machine_state);
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
