#pragma once
#include "backend/machine_state.h"
#include "backend/number_util.h"

#define default_window_flags (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)

namespace SicSimUi {
    enum class MachineState {
        Running,
        RunningTo,
        Paused,
    };

    enum class ControlPanelAction {
        None,
        Step,
        Run,
        RunTo,
        Pause,
        LoadFile,
    };


    class RenderState {
    public:
        ControlPanelAction control_panel_action;
        MachineState control_panel_state;
        SicSimBackend::MachineState &machine_state;
        SicSimBackend::uint24_t run_to_address;
        uint32_t update_frequency;
        bool machine_state_changed;
        std::string filename;
        RenderState(SicSimBackend::MachineState &machine_state) : 
            control_panel_action(ControlPanelAction::None),
            control_panel_state(MachineState::Paused), 
            machine_state(machine_state),
            run_to_address(0),
            update_frequency(100),
            machine_state_changed(true),
            filename("")
        {}
    };

    void main_render_view(RenderState &render_state);
} // namespace SicSimUi
