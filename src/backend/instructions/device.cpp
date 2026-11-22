#include "device.h"
#include "backend/devices/device.h"

namespace SicSimBackend {
    ModificationList OpReadDevice::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        Device *device = get_device(decoded.value.get_unsigned() & 0xFF);
        uint8_t value = device->read();
        uint8_t new_a = (machine_state.rA.get_unsigned() & 0xFFFFFF00) | value;
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::A, 0), new_a));
        return modifications;
    }
    std::string OpReadDevice::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "RD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpWriteDevice::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        Device *device = get_device(decoded.value.get_unsigned() & 0xFF);
        device->write(machine_state.rA.get_unsigned() & 0xFF);
        return modifications;
    }
    std::string OpWriteDevice::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "WD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }

    ModificationList OpTestDevice::handle(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        Format34Instruction decoded = decode_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state);
        ModificationList modifications;
        Device *device = get_device(decoded.value.get_unsigned() & 0xFF);
        bool ready = device->is_ready();
        bool new_cc = ready ? static_cast<uint8_t>(CCState::LT) : static_cast<uint8_t>(CCState::EQ);
        uint32_t newSW = (machine_state.rSW.get_unsigned() & 0xFCFFFF) | (new_cc << 16);
        modifications.push_back(std::make_pair(std::make_pair(ModificationTarget::SW, 0), newSW));
        return modifications;
    }
    std::string OpTestDevice::display_instruction(uint32_t instruction, uint24_t instruction_address, MachineState &machine_state) {
        std::string out = "TD " + display_format34(reinterpret_cast<uint8_t *>(&instruction), instruction_address, machine_state).address_str;
        return out;
    }
} // namespace SicSimBackend
