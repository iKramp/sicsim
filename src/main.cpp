#include "backend/execution_loop.h"
#include "backend/instructions/instruction.h"
#include "backend/machine_state.h"
#include "ui/mod.h"
#include "ui/gpu.h"
#include "backend/loader.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Initing UI..." << std::endl;
    SicSimBackend::init_instruction_handler_table();
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

    std::cout << "Starting main loop..." << std::endl;
    while (!SicSimUi::should_close_ui()) {
        SicSimUi::update_ui(render_state, SicSimUi::main_render_view);

        bool step = false;

        if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Step) {
            render_state.control_panel_action = SicSimUi::ControlPanelAction::None;
            step = true;
        } else if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Run) {
            render_state.control_panel_state = SicSimUi::ControlPanelState::Running;
        } else if (render_state.control_panel_action == SicSimUi::ControlPanelAction::Pause) {
            render_state.control_panel_state = SicSimUi::ControlPanelState::Paused;
        }

        if (step || render_state.control_panel_state == SicSimUi::ControlPanelState::Running) {
            render_state.machine_state_changed = true;

            SicSimBackend::StepResult step_result = SicSimBackend::step_sim();
            if (step_result == SicSimBackend::StepResult::HALT) {
                std::cout << "Program halted." << std::endl;
                render_state.control_panel_state = SicSimUi::ControlPanelState::Paused;
            } else if (step_result == SicSimBackend::StepResult::INVALID_INSTRUCTION) {
                std::cout << "Invalid instruction encountered." << std::endl;
                render_state.control_panel_state = SicSimUi::ControlPanelState::Paused;
            }
        } else {
            render_state.machine_state_changed = false;
        }
    }
    SicSimUi::cleanup_ui();
    return 0;
}
