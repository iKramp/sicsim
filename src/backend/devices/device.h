#pragma once
#include <cstdint>

namespace SicSimBackend {
    class Device {
    public:
        virtual uint8_t read() = 0;
        virtual uint8_t peek() = 0;
        virtual void write(uint8_t value) = 0;
        virtual bool is_ready() = 0;
    };

    Device *get_device(uint8_t device_id);
} // namespace SicSimBackend
