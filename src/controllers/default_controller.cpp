//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller.h"

// TODO: macro this
static Instruction instructions[] = {
        {0x0, nullptr, "null"},
        {0x1, static_cast<Handler>(&DefaultController::cmd_end), "cmd_end"},
        {0x2, static_cast<Handler>(&DefaultController::cmd_call), "cmd_call"},
        {0x3, static_cast<Handler>(&DefaultController::cmd_jmp), "cmd_jmp"},
};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

DefaultController::DefaultController():
    DefaultControllerBase(0x0, ::instructions, instructions_cnt) {}

void DefaultController::get_handler(Handler *out, uint32_t index) {
    *out = instructions[index].handler;
}

void DefaultController::cmd_end_logger() {
    // Do nothing
}

void DefaultController::cmd_end(BasePacContext *ctx, int sec_id, int instr_id) {

}

void DefaultController::cmd_call(BasePacContext *ctx, int sec_id, int instr_id) {

}

void DefaultController::cmd_jmp(BasePacContext *ctx, int sec_id, int instr_id) {

}