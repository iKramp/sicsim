#pragma once
#include "backend/machine_state.h"

#define default_window_flags (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)

namespace SicSimUi {
    void main_render_view(SicSimBackend::MachineState &machine_state);
}
