#include "control_panel.h"
#include "imgui.h"
#include "ui/mod.h"

static char *runnnig_to_string = nullptr;

namespace SicSimUi {
    ControlPanelAction render_control_panel(RenderState &render_state) {
        if (runnnig_to_string == nullptr) {
            runnnig_to_string = new char[7];
            strcpy(runnnig_to_string, "000000");
        }

        bool step_enabled = true;
        bool running = false;

        bool start_pause_pressed;
        bool step_pressed;

        switch (render_state.control_panel_state) {
        case MachineState::Running:
            step_enabled = false;
            running = true;
            break;
        case MachineState::Paused:
            step_enabled = true;
            running = false;
            break;
        case MachineState::RunningTo:
            step_enabled = false;
            running = true;
            break;
        }

        unsigned char *start_pause_label = running ? (unsigned char *)"Pause" : (unsigned char *)"Start";

        start_pause_pressed = ImGui::Button((const char *)start_pause_label);

        if (!step_enabled) {
            ImGui::BeginDisabled();
        }
        step_pressed = ImGui::Button("Step");

        ImVec2 text_size = ImGui::CalcTextSize("000000");
        ImGui::PushItemWidth(text_size.x * 1.05);

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_NoHorizontalScroll;
        bool running_to_edited = ImGui::InputText("##running_to_input", runnnig_to_string, strlen("000000") + 1, flags);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        bool run_to = ImGui::Button("Run To");
        if (!step_enabled) {
            ImGui::EndDisabled();
        }

        ImGui::InputInt("Frequency (Hz)", (int *)&render_state.update_frequency);
        ImGui::Text("Open file: %s", render_state.filename.c_str());

        if (running_to_edited) {
            uint32_t new_val = strtoul(runnnig_to_string, nullptr, 16);
            render_state.run_to_address.set_value(new_val, 24, false);
        }

        if (start_pause_pressed) {
            if (running) {
                return ControlPanelAction::Pause;
            } else {
                return ControlPanelAction::Run;
            }
        } else if (step_pressed && step_enabled) {
            return ControlPanelAction::Step;
        } else if (run_to) {
            return ControlPanelAction::RunTo;
        } else {
            return ControlPanelAction::None;
        }


    }
} // namespace SicSimUi
