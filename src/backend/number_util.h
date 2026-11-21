#pragma once

#include <cstdint>

namespace SicSimBackend {
    //24B numbers

    class uint24_t {
        uint32_t value;

        public:
        uint24_t() : value(0) {}
        uint24_t(uint64_t v) : value(v & 0xFFFFFF) {}

        uint32_t get_signed();
        uint32_t get_unsigned();

        uint24_t add(uint24_t other);
        uint24_t sub(uint24_t other);
        uint24_t mul(uint24_t other);
        uint24_t div(uint24_t other);
        uint24_t mod(uint24_t other);
        uint24_t bit_and(uint24_t other);
        uint24_t bit_or(uint24_t other);
        uint24_t shl(uint8_t n);
        uint24_t shr(uint8_t other, bool is_signed);

        void set_value(uint32_t bits, uint8_t n_bits, bool is_signed);
    };

    class float48_t {
        uint64_t value;

        public:
        float48_t() : value(0) {}
        float48_t(uint64_t v) : value(v & 0xFFFFFFFFFFFF) {}
    };
}
