#pragma once
#include <cstdint>

namespace SicSimBackend {
    struct DeviceTarget {
        uint8_t device_id;
        uint8_t write_flag;

        DeviceTarget(uint8_t device_id, uint8_t write_flag) : device_id(device_id), write_flag(write_flag) {}
        DeviceTarget(uint32_t target) : device_id((target >> 8) & 0xFF), write_flag(target & 0xFF) {}
        uint32_t to_uint32() const {
            return (static_cast<uint32_t>(device_id) << 8) | static_cast<uint32_t>(write_flag);
        }
    };

    class Device {
    public:
        virtual uint8_t read() = 0;
        virtual uint8_t peek() = 0;
        virtual void write(uint8_t value) = 0;
        virtual bool is_ready() = 0;
    };

    Device *get_device(uint8_t device_id);
} // namespace SicSimBackend
