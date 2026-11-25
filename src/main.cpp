#include "backend/devices/device.h"
#include "backend/execution_loop.h"
#include "backend/instructions/instruction.h"
#include "backend/loader.h"
#include "backend/machine_state.h"
#include "ui/gpu.h"
#include "ui/mod.h"
#include <chrono>
#include <iostream>

void print_stats(std::chrono::system_clock::time_point start_time, std::chrono::system_clock::time_point end_time, int num_instructions) {
    // print elapsed miliseconds, steps, steps per second
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
    double elapsed_ms = elapsed.count();
    double steps_per_second = (double)num_instructions / (elapsed_ms / 1000.0);
    std::cout << "Elapsed time: " << elapsed_ms << " ms" << std::endl;
    std::cout << "Number of instructions executed: " << num_instructions << std::endl;
    std::cout << "Steps per second: " << steps_per_second << std::endl;
}

int main(int argc, char *argv[]) {
    std::cout << "Initing UI..." << std::endl;
    SicSimBackend::init_instruction_handler_table();
    SicSimBackend::init_devices();
    int ret = SicSimUi::init_ui();
    if (ret != 0) {
        SicSimUi::cleanup_ui();
        return ret;
    }

    std::cout << "Initing execution loop..." << std::endl;
    SicSimBackend::init_execution_loop();
    SicSimBackend::MachineState &machine_state = SicSimBackend::get_machine_state();

    if (argc == 2) {
        std::string filename = argv[1];
        std::cout << "Loading program: " << filename << std::endl;
        SicSimBackend::load_program(filename, machine_state);
    }

    SicSimUi::RenderState render_state = SicSimUi::RenderState(machine_state);

    std::chrono::system_clock::time_point last_time = std::chrono::system_clock::now();
    uint32_t steps = 0;

    std::cout << "Starting main loop..." << std::endl;
    while (!SicSimUi::should_close_ui()) {
        SicSimUi::update_ui(render_state, SicSimUi::main_render_view);

        bool step = false;

        if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Step) {
            render_state.control_panel_action = SicSimUi::ControlPanelAction::None;
            step = true;
        } else if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Run) {
            render_state.control_panel_state = SicSimUi::MachineState::Running;
            last_time = std::chrono::system_clock::now();
            steps = 0;
        } else if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Pause) {
            render_state.control_panel_state = SicSimUi::MachineState::Paused;
            print_stats(last_time, std::chrono::system_clock::now(), steps);
        } else if (render_state.control_panel_action == SicSimUi::ControlPanelAction::RunTo) {
            render_state.control_panel_state = SicSimUi::MachineState::RunningTo;
            last_time = std::chrono::system_clock::now();
            steps = 0;
        }

        if (step || render_state.control_panel_state == SicSimUi::MachineState::Running ||
            render_state.control_panel_state == SicSimUi::MachineState::RunningTo) {
            render_state.machine_state_changed = true;

            SicSimUi::set_framerate_limit(0);

            uint32_t step_size = render_state.update_frequency / 100;

            if (render_state.update_frequency < 100) {
                step_size = 1;
                SicSimUi::set_framerate_limit(render_state.update_frequency);
            }

            SicSimBackend::StepResult step_result = SicSimBackend::step_sim(step ? 1 : step_size, -1);
            // SicSimBackend::StepResult step_result = SicSimBackend::step_sim(1, -1);
            steps += 1;
            if (step_result == SicSimBackend::StepResult::HALT) {
                std::cout << "Execution stopped" << std::endl;
                render_state.control_panel_state = SicSimUi::MachineState::Paused;
                print_stats(last_time, std::chrono::system_clock::now(), steps);
            } else if (step_result == SicSimBackend::StepResult::INVALID_INSTRUCTION) {
                std::cout << "Invalid instruction encountered." << std::endl;
                render_state.control_panel_state = SicSimUi::MachineState::Paused;
                print_stats(last_time, std::chrono::system_clock::now(), steps);
            }
        } else {
            render_state.machine_state_changed = false;
            SicSimUi::set_framerate_limit(100);
        }
    }
    SicSimUi::cleanup_ui();
    return 0;
}
