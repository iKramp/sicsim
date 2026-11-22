#include "mod.h"
#include "gpu.h"
#include "backend/machine_state.h"
#include "imgui_internal.h"
#include "ui/memory_view.h"
#include "ui/register_view.h"
#include <stdlib.h> // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

static bool show_demo_window = true;

namespace SicSimUi {
    void main_render_view(SicSimBackend::MachineState &machine_state) {
        ImGui::NewFrame();

        ImGuiID dockspace_id = ImGui::GetID("My Dockspace");

        ImGuiViewport *viewport = ImGui::GetMainViewport();

        if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
            ImGui::DockBuilderAddNode(
                dockspace_id, 
                ImGuiDockNodeFlags_DockSpace | 
                ImGuiDockNodeFlags_NoTabBar | 
                ImGuiDockNodeFlags_NoWindowMenuButton | 
                ImGuiDockNodeFlags_NoCloseButton | 
                ImGuiDockNodeFlags_NoResize |
                ImGuiDockNodeFlags_NoDockingInCentralNode |
                ImGuiDockNodeFlags_NoDockingSplit
            );
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_main = dockspace_id;
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);
            ImGuiID dock_id_left_top = 0;
            ImGuiID dock_id_left_bottom = 0;
            ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.80f, &dock_id_left_top, &dock_id_left_bottom);
            ImGui::DockBuilderDockWindow("Hex", dock_id_main);
            ImGui::DockBuilderDockWindow("Registers", dock_id_left_top);
            ImGui::DockBuilderDockWindow("Dear ImGui Demo", dock_id_left_bottom);
            ImGui::DockBuilderFinish(dockspace_id);
        }
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);

        // ImVec2 window_size = io->DisplaySize;
        // ImGui::SetNextWindowSize(window_size, ImGuiCond_Always);
        // ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::Begin("Hex", nullptr, default_window_flags);
        // ImGui::Begin("Hex");
        SicSimUi::render_memory_view(SicSimBackend::get_memory_pointer(0));
        ImGui::End();

        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        SicSimUi::render_register_view(machine_state, false);
    }
} // namespace SicSimUi
