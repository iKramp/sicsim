#pragma once
#include "backend/machine_state.h"
#include "ui/control_panel.h"

#define default_window_flags (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)

namespace SicSimUi {
    class RenderState {
    public:
        ControlPanelAction control_panel_action;
        ControlPanelState control_panel_state;
        SicSimBackend::MachineState &machine_state;
        bool machine_state_changed;
        RenderState(SicSimBackend::MachineState &machine_state) : control_panel_state(ControlPanelState::Paused), machine_state(machine_state) {}
    };

    void main_render_view(RenderState &render_state);
} // namespace SicSimUi
