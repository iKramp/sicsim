#include "backend/execution_loop.h"
#include "backend/machine_state.h"
#include "ui/mod.h"
#include <cstdlib>

int main() {
    int ret = SicSimUi::init_ui();
    if (ret != 0) {
        SicSimUi::cleanup_ui();
        return ret;
    }

    SicSimBackend::init_execution_loop();
    SicSimBackend::MachineState &machine_state = SicSimBackend::get_machine_state();

    for (int i = 0; i < 0x1000; i++) {
        SicSimBackend::write_memory_byte(i, rand());
    }

    while (!SicSimUi::should_close_ui()) {
        SicSimUi::update_ui(machine_state);
    }
    SicSimUi::cleanup_ui();
    return 0;
}
