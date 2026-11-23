#pragma once
#include "backend/machine_state.h"

namespace SicSimUi {
    enum class ControlPanelState {
        Running,
        Paused,
    };

    enum class ControlPanelAction {
        None,
        Step,
        Run,
        Pause
    };

    ControlPanelAction render_control_panel(ControlPanelState state);
}
