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
{
    auto out = ctx->getArgValuePtr(0, 1, 4);
    auto label_arg = ctx->getArgValuePtr(1, 0, 4);
    if (context) {
        out->as_int = label_arg->index;
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, createScript)
{
    auto PAC_start = ctx->getArgValuePtr(0, 1, 4);
    auto PAC_offset = ctx->getArgValuePtr(1, 1, 4);
    auto group_id = ctx->getArgValuePtr(2, 1, 4);
    auto script_arg_1 = ctx->getArgValuePtr(3, 1, 4);
    auto script_arg_2 = ctx->getArgValuePtr(4, 1, 4);
    auto out = ctx->getArgValuePtr(5, 1, 4);

    auto type_5 = ctx->get_arg_type(5);

    if (context) {
        auto script_manager = context->script_manager;
        PacContext* created = nullptr;
        // TODO: implement the script managers
        // created = script_manager->create_script(
        //      PAC_start->as_int, PAC_offset->as_int,
        //      1, 0,
        //      script_arg_1->as_int, script_arg_2->as_int,
        // );

        created->group_id = group_id->as_int;
        created->parent_ctx = context->get_root_context();
        created->run(1);

        if (is_int_reg(type_5)) {
            uint32_t storage = 0;
            Debugger::remapper.RemapAddress(created, &storage);
            out->as_int = storage;
        }
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, setGroupId)
{
    auto group_id = ctx->getArgValuePtr(0, 1, 4);
    if (context) {
        context->group_id = group_id->as_int;
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, getGroupId)
{
    auto out = ctx->getArgValuePtr(0, 1, 4);
    out->as_int = 0xffffffff;

    if (context) {
        out->as_int = context->group_id;
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, getParentGroupId)
{
    auto out = ctx->getArgValuePtr(0, 1, 4);
    out->as_int = 0xffffffff;
    if (context) {
        auto parent = context->parent_ctx;
        if (parent) {
            out->as_int = parent->group_id;
        }
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, killChildren)
{
    auto script_manager = context->script_manager;
    if (script_manager) {
        // TODO: implement the script managers
        // script_manager->kill_children(context);
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, killScript)
{
    auto group_id = ctx->getArgValuePtr(0, 1, 4);
    auto script_manager = context->script_manager;
    if (script_manager) {
        // TODO: implement the script managers
        // script_manager->kill_script(group_id->as_int);
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, setAddress)
{
    auto group_id = ctx->getArgValuePtr(0, 1, 4);
    auto PAC_offset = ctx->getArgValuePtr(1, 1, 4);
    if (context) {
        auto script_manager = context->script_manager;
        if (script_manager) {
            // script_manager->set_address(group_id->as_int, PAC_offset->as_int);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, setLocalValue)
{
    auto ctx_arg = ctx->getArgValuePtr(0, 1, 4);
    // Discarded on purpose, to reacquire it again via get_arg_value
    ctx->getArgValuePtr(1, 1, 4);
    auto value_arg = ctx->getArgValuePtr(2, 1, 4);

    auto type_1 = ctx->get_arg_type(1);
    auto type_2 = ctx->get_arg_type(2);

    auto index = ctx->get_arg_value(1);

    PacContext* script_context = nullptr;
    Debugger::remapper.RemapAddress(ctx_arg->as_int, &script_context);

    if (script_context) {
        auto locals = script_context->get_locals();

        if (is_local_float_reg(type_1)) {
            auto value = cast_arg_to_float(type_2, value_arg);
            // For some reason we use the integer representation here
            locals->set_int_reg(index.as_int & 0xFF, value);
        }

        if (is_local_int_reg(type_1)) {
            auto value = cast_arg_to_int(type_2, value_arg);
            locals->set_float_reg(index.as_int & 0xFF, value);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(ScriptController, getLocalValue)
{
    auto ctx_arg = ctx->getArgValuePtr(0, 1, 4);
    // Discarded on purpose, to reacquire it again via get_arg_value
    ctx->getArgValuePtr(1, 1, 4);
    auto out = ctx->getArgValuePtr(2, 1, 4);

    auto type_1 = ctx->get_arg_type(1);
    auto type_2 = ctx->get_arg_type(2);

    auto index = ctx->get_arg_value(1);

    PacContext* script_context = nullptr;
    Debugger::remapper.RemapAddress(ctx_arg->as_int, &script_context);

    if (script_context) {
        auto locals = script_context->get_locals();

        if (is_local_float_reg(type_1)) {
            auto value = locals->get_float_reg(index.as_int & 0xFF);
            if (is_not_float_arg(type_2)) {
                out->as_int = value;
            }
            else {
                out->as_float = value;
            }
        }

        if (is_local_int_reg(type_1)) {
            auto value = locals->get_int_reg(index.as_int & 0xFF);
            if (is_not_float_arg(type_2)) {
                out->as_int = value;
            }
            else {
                out->as_float = value;
            }
        }
    }

    ctx->setCmdId(0);
}
