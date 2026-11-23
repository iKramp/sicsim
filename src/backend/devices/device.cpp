#include "device.h"
#include <cstdio>

static SicSimBackend::Device *devices[256] = {nullptr};

class TestDevice : public SicSimBackend::Device {
public:
    uint8_t read() override {
        return getchar();
    }
    uint8_t peek() override {
        uint8_t value = read();
        ungetc(value, stdin);
        return value;
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

