//
// Created by Nemoumbra on 06.01.2025.
//

#include "script_controller.h"
#include "../../context/base_context.h"
#include "../../debugger/debugger.h"


static Instruction instructions[] = {
        REGISTER_TABLE_BEGIN
        REGISTER_TABLE_CMD(ScriptController, 0x1, dummy)
        REGISTER_TABLE_CMD(ScriptController, 0x2, setScriptLabelTable)
        REGISTER_TABLE_CMD(ScriptController, 0x3, callScriptLabel)
        REGISTER_TABLE_CMD(ScriptController, 0x4, callScriptLabelId)
        REGISTER_TABLE_CMD(ScriptController, 0x5, setCommandMonitor)
        REGISTER_TABLE_CMD(ScriptController, 0x6, getPack)
        REGISTER_TABLE_CMD(ScriptController, 0x7, labelToInt)
        REGISTER_TABLE_CMD(ScriptController, 0x8, createScript)
        REGISTER_TABLE_CMD(ScriptController, 0x9, setGroupId)
        REGISTER_TABLE_CMD(ScriptController, 0xa, getGroupId)
        REGISTER_TABLE_CMD(ScriptController, 0xb, getParentGroupId)
        REGISTER_TABLE_CMD(ScriptController, 0xc, killChildren)
        REGISTER_TABLE_CMD(ScriptController, 0xd, killScript)
        REGISTER_TABLE_CMD(ScriptController, 0xe, setAddress)
        REGISTER_TABLE_CMD(ScriptController, 0xf, setLocalValue)
        REGISTER_TABLE_CMD(ScriptController, 0x10, getLocalValue)

};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

ScriptController::ScriptController(PacContext* ctx):
        ScriptControllerBase(0x10, ::instructions, instructions_cnt), context(ctx) {}


/// Instructions
COMMAND_IMPLEMENTATION(ScriptController, dummy)
{
    LOG_ME

    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, setScriptLabelTable)
{
    LOG_ME

    auto offset_arg = ctx->getArgValuePtr(0, 0, 4);
    if (context) {
        context->set_script_label_table(offset_arg->as_int);
    }
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, callScriptLabel)
{
    LOG_ME

    ctx->getArgValuePtr(0, 0, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, callScriptLabelId)
{
    LOG_ME

    ctx->getArgValuePtr(0, 1, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, setCommandMonitor)
{
    LOG_ME

    ctx->getArgValuePtr(0, 1, 4);
    ctx->getArgValuePtr(1, 1, 4);
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, getPack)
{
    LOG_ME

    auto out = ctx->getArgValuePtr(0, 1, 4);
    if (context) {
        auto start = context->get_pac_start();
        // NO!!! 8-byte pointers!

        uint32_t value;
        Debugger::remapper.RemapAddress(start, &value);

        out->as_int = value;
    }
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(ScriptController, labelToInt)
{ }
COMMAND_IMPLEMENTATION(ScriptController, createScript)
{ }
COMMAND_IMPLEMENTATION(ScriptController, setGroupId)
{ }
COMMAND_IMPLEMENTATION(ScriptController, getGroupId)
{ }
COMMAND_IMPLEMENTATION(ScriptController, getParentGroupId)
{ }
COMMAND_IMPLEMENTATION(ScriptController, killChildren)
{ }
COMMAND_IMPLEMENTATION(ScriptController, killScript)
{ }
COMMAND_IMPLEMENTATION(ScriptController, setAddress)
{ }
COMMAND_IMPLEMENTATION(ScriptController, setLocalValue)
{ }
COMMAND_IMPLEMENTATION(ScriptController, getLocalValue)
{ }
