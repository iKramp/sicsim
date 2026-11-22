#include "number_util.h"

namespace SicSimBackend {
    uint32_t uint24_t::get_signed() {
        if (value & 0x800000) {
            return value | 0xFF000000;
        } else {
            return value & 0xFFFFFF;
        }
    }

    uint32_t uint24_t::get_unsigned() {
        return value & 0xFFFFFF;
    }

    void uint24_t::set_value(uint32_t bits, uint8_t n_bits, bool is_signed) {
        if (is_signed) {
            bool is_negative = (bits & (1 << (n_bits - 1))) != 0;
            if (is_negative) {
                bits |= ~((1 << n_bits) - 1);
            } else {
                bits &= (1 << n_bits) - 1;
            }
        }
        value = bits & 0xFFFFFF;
    }

    uint24_t uint24_t::add(uint24_t other) {
        return uint24_t((get_signed() + other.get_signed()) & 0xFFFFFF);
    }

    uint24_t uint24_t::sub(uint24_t other) {
        return uint24_t((get_signed() - other.get_signed()) & 0xFFFFFF);
    }

    uint24_t uint24_t::mul(uint24_t other) {
        return uint24_t((get_signed() * other.get_signed()) & 0xFFFFFF);
    }

    uint24_t uint24_t::div(uint24_t other) {
        return uint24_t((get_signed() / other.get_signed()) & 0xFFFFFF);
    }

    uint24_t uint24_t::mod(uint24_t other) {
        return uint24_t((get_signed() % other.get_signed()) & 0xFFFFFF);
    }

    uint24_t uint24_t::bit_and(uint24_t other) {
        return uint24_t(get_unsigned() & other.get_unsigned());
    }

    uint24_t uint24_t::bit_or(uint24_t other) {
        return uint24_t(get_unsigned() | other.get_unsigned());
    }

    uint24_t uint24_t::shl(uint8_t n) {
        return uint24_t((get_unsigned() << n) & 0xFFFFFF);
    }

    uint24_t uint24_t::shr(uint8_t n, bool is_signed) {
        bool is_negative = (get_signed() & 0x800000) != 0;
        uint32_t shifted = get_unsigned() >> n;
        if (is_signed && is_negative) {
            shifted |= ~((1 << (24 - n)) - 1);
        }
        return uint24_t(shifted & 0xFFFFFF);
    }
}
