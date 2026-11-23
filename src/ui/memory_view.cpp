#include "memory_view.h"
#include "backend/machine_state.h"
#include "extensions/imgui_hex_editor/imgui_hex.h"

static ImGuiHexEditorState hex_state;

namespace SicSimUi {
    int write_callback(ImGuiHexEditorState *state, int offset, void *data, int size) {
        uint8_t *start_ptr = (uint8_t *)state->Bytes + offset;
        memcpy(start_ptr, data, size);
        SicSimBackend::set_max_written_address(offset + size, false);
        return size;
    }

    void render_memory_view(uint8_t *data) {
        hex_state.Bytes = data;
        hex_state.MaxBytes = 0x1000;
        hex_state.WriteCallback = write_callback;
        
        ImGui::BeginHexEditor("##Hex editor", &hex_state);
        ImGui::EndHexEditor();
    }
}
