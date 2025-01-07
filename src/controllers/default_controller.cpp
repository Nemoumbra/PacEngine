//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller.h"

DefaultController::DefaultController(): DefaultControllerBase(0) {}

void DefaultController::get_handler(Handler *out, uint32_t index) {
    *out = instructions[index].handler;
}

void DefaultController::cmd_end_logger() {
    // Do nothing
}

//