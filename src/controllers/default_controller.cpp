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
        REGISTER_TABLE_CMD(DefaultController, 0x4, cmd_result)
        REGISTER_TABLE_CMD(DefaultController, 0x5, cmd_resJmp)
        REGISTER_TABLE_CMD(DefaultController, 0x6, cmd_resCall)
        REGISTER_TABLE_CMD(DefaultController, 0x7, cmd_mov)
        REGISTER_TABLE_CMD(DefaultController, 0x8, cmd_add)
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

/// Instructions
COMMAND_IMPLEMENTATION(DefaultController, cmd_end)
{
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

COMMAND_IMPLEMENTATION(DefaultController, cmd_jmp)
{
    LOG_ME

    auto dest = ctx->getArgValuePtr(0, 0, 4);
    ctx->debug_logger(0);
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_call)
{
    LOG_ME

    auto dest = ctx->getArgValuePtr(0, 0, 4);
    ctx->debug_logger(0);
    ctx->save_return_address();
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0x0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_result)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    ctx->debug_logger(0);
    ctx->set_result(arg->as_int != 0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_resJmp)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    auto dest = ctx->getArgValuePtr(1, 0, 4);
    ctx->debug_logger(0);
    ctx->debug_logger(1);
    auto value = arg->as_int;
    auto result = ctx->get_result();
    if ((value != 0) == result) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_resCall)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    auto dest = ctx->getArgValuePtr(1, 0, 4);
    ctx->debug_logger(0);
    ctx->debug_logger(1);
    auto value = arg->as_int;
    auto result = ctx->get_result();
    if ((value != 0) == result) {
        ctx->save_return_address();
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_mov)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    if (is_not_float_arg(type_1)) {
        if (is_not_float_arg(type_2)) {
            arg_1->as_int = arg_2->as_int;
        }
        else {
            arg_1->as_int = arg_2->as_float;
        }
    }
    else {
        if (is_not_float_arg(type_2)) {
            arg_1->as_float = arg_2->as_int;
        }
        else {
            arg_1->as_float = arg_2->as_float;
        }
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_add)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    if (is_not_float_arg(type_1)) {
        if (is_not_float_arg(type_2)) {
            arg_1->as_int += arg_2->as_int;
        }
        else {
            arg_1->as_int += arg_2->as_float;
        }
    }
    else {
        if (is_not_float_arg(type_2)) {
            arg_1->as_float += arg_2->as_int;
        }
        else {
            arg_1->as_float += arg_2->as_float;
        }
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_sub)
{

}
COMMAND_IMPLEMENTATION(DefaultController, cmd_mul)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_div)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_inc)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_dec)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_loop)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_waitFrame)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_waitTime)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_rand)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_iand)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ior)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ixor)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_irol)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_iror)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifEQ)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifNE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLSE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLBE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLS)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLB)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifAND)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifOR)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallEQ)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallNE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLSE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLBE)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLS)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLB)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallAND)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallOR)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgSet)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgClr)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgSetId)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgClrId)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMov)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgAND)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgOR)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgZERO)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_inxJmp)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_stkDec)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_stkClr)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_break)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_string)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_print)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_textOut)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_setSleep)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_sinf)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_cosf)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_atan2f)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_abs)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_sqrt)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_setLabelId)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_callLabelId)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_jmpLabelId)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_callLabel)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_jmpLabel)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_mod)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_memset)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_memcpy)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMemset)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMemcpy)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_stringEnable)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_srand)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_initArray)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_addArray)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_setArray)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_getArray)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_F32toF16)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_F16toF32)
{ }
COMMAND_IMPLEMENTATION(DefaultController, cmd_getElapsedTime)
{ }
