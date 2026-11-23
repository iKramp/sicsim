#include "register_view.h"
#include "backend/instructions/instruction.h"
#include "backend/machine_state.h"
#include "backend/register.h"
#include "imgui.h"
#include <cstring>
#include <array>

#define NUM_REGISTERS 10

static char *render_string[NUM_REGISTERS];

static std::string name_strings[NUM_REGISTERS] = {
    "A:  0x", 
    "X:  0x", 
    "L:  0x", 
    "B:  0x", 
    "S:  0x", 
    "T:  0x",
    "",
    "",
    "PC: 0x",
    "SW: 0x"
};

static int render_string_length = strlen("PC: 0x") + 1; // +1 for null terminator

namespace SicSimUi {
    void render_register_view(SicSimBackend::MachineState &machine_state, bool changed) {
        ImVec2 text_size = ImGui::CalcTextSize("000000");
        uint32_t instruction_address = machine_state.PC.get_unsigned();
        std::pair<std::array<uint8_t, 4>, bool> instruction = SicSimBackend::read_memory_instruction(instruction_address);

        SicSimBackend::InstructionHandler *handler = SicSimBackend::get_instruction_handler(instruction.first[0]);

        SicSimBackend::ModificationList modifications;
        if (handler != nullptr) {
            modifications = handler->handle(instruction.first, instruction_address, machine_state);
            uint32_t instruction_length = handler->n_bytes(instruction.first, instruction_address, machine_state);
            modifications.push_back({{SicSimBackend::ModificationTarget::PC, 0}, instruction_address + instruction_length});
        }

        SicSimBackend::uint24_t invalid_reg = 0;
        SicSimBackend::uint24_t *regs[NUM_REGISTERS] = {
            &machine_state.rA, 
            &machine_state.rX, 
            &machine_state.rL,  
            &machine_state.rB, 
            &machine_state.rS,
            &machine_state.rT, 
            &invalid_reg, //F
            &invalid_reg, //empty
            &machine_state.PC,
            &machine_state.rSW 
        };


        if (*render_string == nullptr) {
            for (int i = 0; i < NUM_REGISTERS; ++i) {
                render_string[i] = new char[render_string_length];
                snprintf(render_string[i], render_string_length, "%06X", regs[i]->get_unsigned());
            }
        }

        ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_NoHorizontalScroll;

        for (int i = 0; i < NUM_REGISTERS; ++i) {
            if (name_strings[i].empty()) {
                continue;
            }
            if (changed) {
                snprintf(render_string[i], render_string_length, "%06X", regs[i]->get_unsigned());
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

            ImGui::PushItemWidth(text_size.x * 1.05);
            bool edited = ImGui::InputText("##", render_string[i], render_string_length, flags);
            ImGui::PopItemWidth();

            //loop through modification list
            for (const auto &modification : modifications) {
                SicSimBackend::Register reg = static_cast<SicSimBackend::Register>(i);
                SicSimBackend::ModificationTarget reg_target = SicSimBackend::reg_into_modification_target(reg);
                if (modification.first.first == reg_target) {
                    ImGui::SameLine();
                    ImGui::Text(" <- 0x%06X", (unsigned int)modification.second);
                    break;
                }
            }

            ImGui::PopID();


            //value change
            if (edited) {
                uint32_t new_val = strtoul(render_string[i], nullptr, 16);
                regs[i]->set_value(new_val, 24, false);
            }

            style.FramePadding = frame_padding;
            style.ItemSpacing = item_spacing;
            style.ItemInnerSpacing = item_inner_spacing;
        }
    }
} // namespace SicSimUi
