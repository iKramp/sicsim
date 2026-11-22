#include "register_view.h"
#include "mod.h"
#include "imgui.h"
#include <cstring>

#define NUM_REGISTERS 8

static char *render_string[NUM_REGISTERS];

static std::string name_strings[NUM_REGISTERS] = {
    "A:  0x", 
    "X:  0x", 
    "L:  0x", 
    "S:  0x", 
    "T:  0x", 
    "B:  0x", 
    "SW: 0x",
    "PC: 0x"
};

static int render_string_length = strlen("PC: 0x") + 1; // +1 for null terminator

namespace SicSimUi {
    void render_register_view(SicSimBackend::MachineState &machine_state, bool changed) {
        SicSimBackend::uint24_t *regs[NUM_REGISTERS] = {
            &machine_state.rA, 
            &machine_state.rX, 
            &machine_state.rL,  
            &machine_state.rS,
            &machine_state.rT, 
            &machine_state.rB, 
            &machine_state.rSW, 
            &machine_state.PC
        };

        if (*render_string == nullptr) {
            for (int i = 0; i < 8; ++i) {
                render_string[i] = new char[render_string_length];
                snprintf(render_string[i], render_string_length, "%06X", regs[i]->get_unsigned());
            }
        }

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase;

        ImGui::Begin("Registers", nullptr, default_window_flags);
        for (int i = 0; i < NUM_REGISTERS; ++i) {
            if (changed) {
                snprintf(render_string[i], render_string_length, name_strings[i].c_str(), regs[i]->get_unsigned());
            }

            ImGuiStyle &style = ImGui::GetStyle();
            ImVec2 frame_padding = style.FramePadding;
            ImVec2 item_spacing = style.ItemSpacing;
            ImVec2 item_inner_spacing = style.ItemInnerSpacing;

            style.FramePadding = ImVec2();
            style.ItemSpacing = ImVec2();


            ImGui::PushID(i);
            ImGui::TextUnformatted(name_strings[i].c_str());
            ImGui::SameLine();
            ImVec2 text_size = ImGui::CalcTextSize(render_string[i]);
            ImGui::PushItemWidth(text_size.x);



            bool edited = ImGui::InputText("##", render_string[i], render_string_length, flags);




            ImGui::PopItemWidth();
            if (edited) {
                uint32_t new_val = strtoul(render_string[i], nullptr, 16);
                regs[i]->set_value(new_val, 24, false);
            }

            ImGui::PopID();

            style.FramePadding = frame_padding;
            style.ItemSpacing = item_spacing;
            style.ItemInnerSpacing = item_inner_spacing;
        }

        ImGui::End();
    }
} // namespace SicSimUi
