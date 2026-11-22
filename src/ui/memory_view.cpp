#include "memory_view.h"
#include "extensions/imgui_hex_editor/imgui_hex.h"

static ImGuiHexEditorState hex_state;

namespace SicSimUi {
    void render_memory_view(uint8_t *data) {
        hex_state.Bytes = data;
        hex_state.MaxBytes = 0x1000;
        ImGui::Begin("hex editor");
        
        ImGui::BeginHexEditor("##Hex editor", &hex_state);
        ImGui::EndHexEditor();

        ImGui::End();
        
    }
}
