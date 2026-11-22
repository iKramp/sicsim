#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "backend/machine_state.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

namespace SicSimUi {
    int init_ui();
    // pass in callback to actually render
    void update_ui(SicSimBackend::MachineState &machine_state, void (*render_callback)(SicSimBackend::MachineState &machine_state));
    void cleanup_ui();
    bool should_close_ui();

    GLFWwindow *get_window();
    ImGuiIO *get_io();
} // namespace SicSimUi
