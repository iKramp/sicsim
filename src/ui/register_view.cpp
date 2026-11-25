#include "register_view.h"
#include "backend/devices/device.h"
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

static int render_string_length = strlen("000000") + 1;

namespace SicSimUi {
    void render_register_view(SicSimBackend::MachineState &machine_state, bool changed) {
        ImVec2 text_size = ImGui::CalcTextSize("000000");
        uint32_t instruction_address = machine_state.PC.get_unsigned();
        std::pair<std::array<uint8_t, 4>, bool> instruction = SicSimBackend::read_memory_instruction(instruction_address);

        SicSimBackend::InstructionHandler *handler = SicSimBackend::get_instruction_handler(instruction.first[0]);

        std::string instruction_string = "";

        SicSimBackend::ModificationList modifications;
        if (handler != nullptr) {
            modifications = handler->handle(instruction.first, instruction_address, machine_state);
            uint32_t instruction_length = handler->n_bytes(instruction.first, instruction_address, machine_state);
            modifications.push_back({{SicSimBackend::ModificationTarget::PC, 0}, instruction_address + instruction_length});
            instruction_string = handler->display_instruction(instruction.first, instruction_address, machine_state);
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

        //find mem edit
        bool mem_edit = false;
        for (const auto &modification : modifications) {
            if (mem_edit) {
                break;
            }
            if (modification.first.first == SicSimBackend::ModificationTarget::MEM1B) {
                ImGui::Text("MEM[%06X]       <- %06X", (unsigned int)modification.first.second.get_unsigned(), (unsigned int)modification.second);
                mem_edit = true;
            } else if (modification.first.first == SicSimBackend::ModificationTarget::MEM3B) {
                ImGui::Text("MEM[%06X..%06X] <- %06X", (unsigned int)modification.first.second.get_unsigned(), (unsigned int)(modification.first.second.get_unsigned() + 3), (unsigned int)modification.second);
                mem_edit = true;
            } else if (modification.first.first == SicSimBackend::ModificationTarget::MEM6B) {
                ImGui::Text("MEM[%06X..%06X] <- %06X", (unsigned int)modification.first.second.get_unsigned(), (unsigned int)(modification.first.second.get_unsigned() + 6), (unsigned int)modification.second);
                mem_edit = true;
            }
        }
        if (!mem_edit) {
            ImGui::Text("MEM");
        }

        //find device read/write
        bool device_rw = false;
        for (const auto &modification : modifications) {
            if (modification.first.first == SicSimBackend::ModificationTarget::DEVICE) {
                device_rw = true;
                SicSimBackend::DeviceTarget target(modification.first.second.get_unsigned());
                if (target.write_flag) {
                    ImGui::Text("Device %02X <- %02X (<- A)", target.device_id, (unsigned int)(modification.second & 0xFF));
                } else {
                    ImGui::Text("Device %02X -> %02X (-> A)", target.device_id, (unsigned int)(modification.second & 0xFF));
                }
                break;
            }
        }
        if (!device_rw) {
            ImGui::Text("Device");
        }

        if (!instruction_string.empty()) {
            ImGui::Text("Instruction: %s", instruction_string.c_str());
        } else {
            ImGui::Text("Instruction: ");
        }
    }
} // namespace SicSimUi
