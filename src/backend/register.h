#pragma once
#include <cstdint>
#include <string>

namespace SicSimBackend {
    enum class Register: uint8_t {
        A = 0,
        X = 1,
        L = 2,
        B = 3,
        S = 4,
        T = 5,
        F = 6,
        PC = 8,
        SW = 9
    };

    enum CCState: uint8_t {
        LT = 0,
        EQ = 1,
        GT = 2,
    };

    std::string get_register_name(Register reg);
}
