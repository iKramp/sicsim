#include "mod.h"
#include "backend/machine_state.h"
#include "imgui_internal.h"
#include "ui/control_panel.h"
#include "ui/memory_view.h"
#include "ui/register_view.h"
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static bool show_demo_window = true;

namespace SicSimUi {
    void main_render_view(RenderState &render_state) {
        ImGui::NewFrame();

        ImGuiID dockspace_id = ImGui::GetID("My Dockspace");

        ImGuiViewport *viewport = ImGui::GetMainViewport();

        if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoTabBar |
                                                        ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton |
                                                        ImGuiDockNodeFlags_NoResize | ImGuiDockNodeFlags_NoDockingInCentralNode |
                                                        ImGuiDockNodeFlags_NoDockingSplit);
            ImGuiID dock_id_main = dockspace_id;
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_hex = 0;
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.30f, &dock_id_left, &dock_id_hex);
            ImGuiID dock_id_reg = 0;
            ImGuiID dock_id_left_bottom = 0;
            ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.20f, &dock_id_reg, &dock_id_left_bottom);
            ImGuiID dock_id_control = 0;
            ImGui::DockBuilderSplitNode(dock_id_left_bottom, ImGuiDir_Up, 0.20f, &dock_id_control, &dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Hex", dock_id_hex);
            ImGui::DockBuilderDockWindow("State", dock_id_reg);
            ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Control Panel", dock_id_control);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::Begin("Hex", nullptr, default_window_flags);
        SicSimUi::render_memory_view(SicSimBackend::get_memory_pointer(0));
        ImGui::End();
        ImGui::Begin("State", nullptr, default_window_flags);
        SicSimUi::render_register_view(render_state.machine_state, render_state.machine_state_changed);
        ImGui::End();
        ImGui::Begin("Control Panel", nullptr, default_window_flags);
        ControlPanelAction action = render_control_panel(render_state);
        ImGui::End();

        render_state.control_panel_action = action;

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
    }
} // namespace SicSimUi
