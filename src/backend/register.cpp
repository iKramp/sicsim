#include "register.h"

namespace SicSimBackend {
    std::string get_register_name(Register reg) {
        switch (reg) {
        case Register::A:
            return "A";
        case Register::X:
            return "X";
        case Register::L:
            return "L";
        case Register::B:
            return "B";
        case Register::S:
            return "S";
        case Register::T:
            return "T";
        case Register::F:
            return "F";
        case Register::PC:
            return "PC";
        case Register::SW:
            return "SW";
        default:
            return "Invalid Register Number";
        }
    }
} // namespace SicSimBackend
