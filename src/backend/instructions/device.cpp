#include "device.h"
#include <array>
#include "backend/devices/device.h"

namespace SicSimBackend {
    ModificationList OpReadDevice::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        Device *device = get_device(decoded.value.get_unsigned() & 0xFF);
        uint8_t new_a = 0;
        if (device) {
            uint8_t value = device->peek();
            new_a = (machine_state.rA.get_unsigned() & 0xFFFFFF00) | value;
        }
        DeviceTarget target = {static_cast<uint8_t>(decoded.value.get_unsigned() & 0xFF), 0};

        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0), new_a));
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::DEVICE, target.to_uint32()), new_a));
        return modifications;
    }
    std::string OpReadDevice::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "RD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpReadDevice::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpWriteDevice::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        DeviceTarget target = {static_cast<uint8_t>(decoded.value.get_unsigned() & 0xFF), 1};
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::DEVICE, target.to_uint32()), machine_state.rA.get_unsigned() & 0xFF));
        return modifications;
    }
    std::string OpWriteDevice::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "WD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpWriteDevice::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }

    ModificationList OpTestDevice::handle(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        Device *device = get_device(decoded.value.get_unsigned() & 0xFF);
        bool ready;
        if (device) {
            ready = device->is_ready();
        } else {
            ready = false;
        }
        bool new_cc = ready ? static_cast<uint8_t>(CCState::LT) : static_cast<uint8_t>(CCState::EQ);
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (new_cc << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpTestDevice::display_instruction(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "TD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
    uint8_t OpTestDevice::n_bytes(std::array<uint8_t, 4> instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        return decoded.extended ? 4 : 3;
    }
} // namespace SicSimBackend
