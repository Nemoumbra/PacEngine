//
// Created by Nemoumbra on 06.01.2025.
//

#include "script_controller.h"
#include "../../context/base_context.h"
#include "../../debugger/debugger.h"


// TODO: generate this from a list of names and the controller's name
static Instruction instructions[] = {
        REGISTER_TABLE_BEGIN
        REGISTER_TABLE_CMD(ScriptController, 0x1, dummy)
        REGISTER_TABLE_CMD(ScriptController, 0x2, setScriptLabelTable)
        REGISTER_TABLE_CMD(ScriptController, 0x3, callScriptLabel)
        REGISTER_TABLE_CMD(ScriptController, 0x4, callScriptLabelId)
        REGISTER_TABLE_CMD(ScriptController, 0x5, setCommandMonitor)
        REGISTER_TABLE_CMD(ScriptController, 0x6, getPack)
};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

ScriptController::ScriptController(BasePacContext* ctx):
        ScriptControllerBase(0x10, ::instructions, instructions_cnt), context(ctx) {}


/// Instructions
void ScriptController::dummy(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    ctx->setCmdId(0x0);
}

void ScriptController::setScriptLabelTable(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    auto offset_arg = ctx->getArgValuePtr(0, 0, 4);
    if (context) {
        // TODO fix the ctx to PacContext*
        // ctx->set_script_label_table(offset_arg->as_int);
    }
    ctx->setCmdId(0x0);
}

void ScriptController::callScriptLabel(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    ctx->getArgValuePtr(0, 0, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

void ScriptController::callScriptLabelId(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    ctx->getArgValuePtr(0, 1, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

void ScriptController::setCommandMonitor(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    ctx->getArgValuePtr(0, 1, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

void ScriptController::getPack(BasePacContext *ctx, int sec_id, int instr_id) {
    LOG_ME

    auto out = ctx->getArgValuePtr(0, 1, 4);
    if (context) {
        auto start = ctx->get_pac_start();
        // NO!!! 8-byte pointers!

        uint32_t value;
        Debugger::remapper.RemapAddress(start, &value);

        out->as_int = value;
    }
    ctx->setCmdId(0x0);
}