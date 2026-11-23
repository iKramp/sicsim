#include "control_panel.h"
#include "imgui.h"

namespace SicSimUi {
    ControlPanelAction render_control_panel(ControlPanelState state) {
        bool step_enabled;
        bool running;

        bool start_pause_pressed;
        bool step_pressed;

        switch (state) {
        case ControlPanelState::Running:
            step_enabled = false;
            running = true;
            break;
        case ControlPanelState::Paused:
            step_enabled = true;
            running = false;
            break;
        }

        unsigned char *start_pause_label = running ? (unsigned char *)"Pause" : (unsigned char *)"Start";

        start_pause_pressed = ImGui::Button((const char *)start_pause_label);

        if (!step_enabled) {
            ImGui::BeginDisabled();
        }
        step_pressed = ImGui::Button("Step");
        if (!step_enabled) {
            ImGui::EndDisabled();
        }

        if (start_pause_pressed) {
            if (running) {
                return ControlPanelAction::Pause;
            } else {
                return ControlPanelAction::Run;
            }
        } else if (step_pressed && step_enabled) {
            return ControlPanelAction::Step;
        } else {
            return ControlPanelAction::None;
        }
    }
} // namespace SicSimUi
