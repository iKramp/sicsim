#pragma once
#include "ui/mod.h"
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "imgui.h"
#include <GLFW/glfw3.h>

namespace SicSimUi {
    int init_ui();
    // pass in callback to actually render
    void update_ui(RenderState &render_state, void (*render_callback)(RenderState &render_state));
    void cleanup_ui();
    bool should_close_ui();

    GLFWwindow *get_window();
    ImGuiIO *get_io();
} // namespace SicSimUi
