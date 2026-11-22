#include "backend/execution_loop.h"
#include "backend/machine_state.h"
#include "ui/mod.h"
#include "ui/gpu.h"
#include <cstdlib>
#include <iostream>

int main() {
    std::cout << "Initing UI..." << std::endl;
    int ret = SicSimUi::init_ui();
    if (ret != 0) {
        SicSimUi::cleanup_ui();
        return ret;
    }

    std::cout << "Initing execution loop..." << std::endl;
    SicSimBackend::init_execution_loop();
    SicSimBackend::MachineState &machine_state = SicSimBackend::get_machine_state();

    std::cout << "Filling memory with random data..." << std::endl;
    for (int i = 0; i < 0x1000; i++) {
        SicSimBackend::write_memory_byte(i, rand());
    }

    std::cout << "Starting main loop..." << std::endl;
    while (!SicSimUi::should_close_ui()) {
        SicSimUi::update_ui(machine_state, SicSimUi::main_render_view);
    }
    SicSimUi::cleanup_ui();
    return 0;
}
