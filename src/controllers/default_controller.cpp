//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller.h"
#include "../context/base_context.h"

#include "../debugger/debugger.h"

// TODO: macro this
static Instruction instructions[] = {
        REGISTER_TABLE_BEGIN
        REGISTER_TABLE_CMD(DefaultController, 0x1, cmd_end)
        REGISTER_TABLE_CMD(DefaultController, 0x2, cmd_jmp)
        REGISTER_TABLE_CMD(DefaultController, 0x3, cmd_call)
};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

DefaultController::DefaultController():
    DefaultControllerBase(0x0, ::instructions, instructions_cnt) {}

void DefaultController::get_handler(Handler *out, uint32_t index) {
    if (index >= this->instr_cnt) {
        Debugger::OnUnknownInstruction(this->section_id, index);
        return;
    }
    *out = instructions[index].handler;
}

void DefaultController::cmd_end_logger() {
    Debugger::OnCmdEnd();
}

void DefaultController::cmd_end(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    auto blocks_count = ctx->get_blocks_count();
    auto undone = ctx->get_undone_calls_count();
    if (undone == 0) {
        if (blocks_count == 0) {
            ctx->set_pac_start(nullptr);
            this->cmd_end_logger();
            ctx->setCmdId(0x0);
        }
        else {
            ctx->revert_to_previous_block();
        }
    }
    else {
        ctx->undo_call();
        ctx->setCmdId(0x0);
    }

}

void DefaultController::cmd_jmp(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    auto dest = ctx->getArgValuePtr(0, 0, 4);
    ctx->debug_logger(0);
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0x0);
}

void DefaultController::cmd_call(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    auto dest = ctx->getArgValuePtr(0, 0, 4);
    ctx->debug_logger(0);
    ctx->save_return_address();
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0x0);
}