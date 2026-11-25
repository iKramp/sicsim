#include "device.h"
#include <cstdio>
#include <iostream>

static SicSimBackend::Device *devices[256] = {nullptr};

class TestDevice : public SicSimBackend::Device {
    bool saved = false;
    uint8_t saved_value = 0;
public:
    uint8_t read() override {
        if (saved) {
            saved = false;
            return saved_value;
        } else {
            return getchar();
        }
    }
    uint8_t peek() override {
        if (saved) {
            return saved_value;
        } else {
            int value = getchar();
            if (value == EOF) {
                return 0;
            }
            saved = true;
            saved_value = static_cast<uint8_t>(value);
            return saved_value;
        }
    }
    void write(uint8_t value) override {
        putchar(value);
        fflush(stdout);
    }
    bool is_ready() override {
        return true;
    }
};



namespace SicSimBackend {
    Device *get_device(uint8_t device_id) {
        return devices[device_id];
    }

    void init_devices() {
        for (int i = 0; i < 256; ++i) {
            devices[i] = new TestDevice();
        }
    }
} // namespace SicSimBackend

