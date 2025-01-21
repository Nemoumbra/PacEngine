//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller.h"
#include "../context/base_context.h"

#include "../debugger/debugger.h"

#include <random>
#include <cmath>
#include <cstring>


static std::mt19937 MT_ctx;
static bool MT_ctx_seeded = false;

static bool str_enabled = true;


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
        REGISTER_TABLE_CMD(DefaultController, 0x9, cmd_sub)
        REGISTER_TABLE_CMD(DefaultController, 0xa, cmd_mul)
        REGISTER_TABLE_CMD(DefaultController, 0xb, cmd_div)
        REGISTER_TABLE_CMD(DefaultController, 0xc, cmd_inc)
        REGISTER_TABLE_CMD(DefaultController, 0xd, cmd_dec)
        REGISTER_TABLE_CMD(DefaultController, 0xe, cmd_loop)
        REGISTER_TABLE_CMD(DefaultController, 0xf, cmd_waitFrame)
        REGISTER_TABLE_CMD(DefaultController, 0x10, cmd_waitTime)
        REGISTER_TABLE_CMD(DefaultController, 0x11, cmd_rand)
        REGISTER_TABLE_CMD(DefaultController, 0x12, cmd_iand)
        REGISTER_TABLE_CMD(DefaultController, 0x13, cmd_ior)
        REGISTER_TABLE_CMD(DefaultController, 0x14, cmd_ixor)
        REGISTER_TABLE_CMD(DefaultController, 0x15, cmd_irol)
        REGISTER_TABLE_CMD(DefaultController, 0x16, cmd_iror)
        REGISTER_TABLE_CMD(DefaultController, 0x17, cmd_ifEQ)
        REGISTER_TABLE_CMD(DefaultController, 0x18, cmd_ifNE)
        REGISTER_TABLE_CMD(DefaultController, 0x19, cmd_ifLSE)
        REGISTER_TABLE_CMD(DefaultController, 0x1a, cmd_ifLBE)
        REGISTER_TABLE_CMD(DefaultController, 0x1b, cmd_ifLS)
        REGISTER_TABLE_CMD(DefaultController, 0x1c, cmd_ifLB)
        REGISTER_TABLE_CMD(DefaultController, 0x1d, cmd_ifAND)
        REGISTER_TABLE_CMD(DefaultController, 0x1e, cmd_ifOR)
        REGISTER_TABLE_CMD(DefaultController, 0x1f, cmd_ifCallEQ)
        REGISTER_TABLE_CMD(DefaultController, 0x20, cmd_ifCallNE)
        REGISTER_TABLE_CMD(DefaultController, 0x21, cmd_ifCallLSE)
        REGISTER_TABLE_CMD(DefaultController, 0x22, cmd_ifCallLBE)
        REGISTER_TABLE_CMD(DefaultController, 0x23, cmd_ifCallLS)
        REGISTER_TABLE_CMD(DefaultController, 0x24, cmd_ifCallLB)
        REGISTER_TABLE_CMD(DefaultController, 0x25, cmd_ifCallAND)
        REGISTER_TABLE_CMD(DefaultController, 0x26, cmd_ifCallOR)
        REGISTER_TABLE_CMD(DefaultController, 0x27, cmd_flgSet)
        REGISTER_TABLE_CMD(DefaultController, 0x28, cmd_flgClr)
        REGISTER_TABLE_CMD(DefaultController, 0x29, cmd_flgSetId)
        REGISTER_TABLE_CMD(DefaultController, 0x2a, cmd_flgClrId)
        REGISTER_TABLE_CMD(DefaultController, 0x2b, cmd_flgMov)
        REGISTER_TABLE_CMD(DefaultController, 0x2c, cmd_flgAND)
        REGISTER_TABLE_CMD(DefaultController, 0x2d, cmd_flgOR)
        REGISTER_TABLE_CMD(DefaultController, 0x2e, cmd_flgZERO)
        REGISTER_TABLE_CMD(DefaultController, 0x2f, cmd_inxJmp)
        REGISTER_TABLE_CMD(DefaultController, 0x30, cmd_stkDec)
        REGISTER_TABLE_CMD(DefaultController, 0x31, cmd_stkClr)
        REGISTER_TABLE_CMD(DefaultController, 0x32, cmd_break)
        REGISTER_TABLE_CMD(DefaultController, 0x33, cmd_string)
        REGISTER_TABLE_CMD(DefaultController, 0x34, cmd_print)
        REGISTER_TABLE_CMD(DefaultController, 0x35, cmd_textOut)
        REGISTER_TABLE_CMD(DefaultController, 0x36, cmd_setSleep)
        REGISTER_TABLE_CMD(DefaultController, 0x37, cmd_sinf)
        REGISTER_TABLE_CMD(DefaultController, 0x38, cmd_cosf)
        REGISTER_TABLE_CMD(DefaultController, 0x39, cmd_atan2f)
        REGISTER_TABLE_CMD(DefaultController, 0x3a, cmd_abs)
        REGISTER_TABLE_CMD(DefaultController, 0x3b, cmd_sqrt)
        REGISTER_TABLE_CMD(DefaultController, 0x3c, cmd_setLabelId)
        REGISTER_TABLE_CMD(DefaultController, 0x3d, cmd_callLabelId)
        REGISTER_TABLE_CMD(DefaultController, 0x3e, cmd_jmpLabelId)
        REGISTER_TABLE_CMD(DefaultController, 0x3f, cmd_callLabel)
        REGISTER_TABLE_CMD(DefaultController, 0x40, cmd_jmpLabel)
        REGISTER_TABLE_CMD(DefaultController, 0x41, cmd_mod)
        REGISTER_TABLE_CMD(DefaultController, 0x42, cmd_memset)
        REGISTER_TABLE_CMD(DefaultController, 0x43, cmd_memcpy)
        REGISTER_TABLE_CMD(DefaultController, 0x44, cmd_flgMemset)
        REGISTER_TABLE_CMD(DefaultController, 0x45, cmd_flgMemcpy)
        REGISTER_TABLE_CMD(DefaultController, 0x46, cmd_stringEnable)
        REGISTER_TABLE_CMD(DefaultController, 0x47, cmd_srand)
        REGISTER_TABLE_CMD(DefaultController, 0x48, cmd_initArray)
        REGISTER_TABLE_CMD(DefaultController, 0x49, cmd_addArray)
        REGISTER_TABLE_CMD(DefaultController, 0x4a, cmd_setArray)
        REGISTER_TABLE_CMD(DefaultController, 0x4b, cmd_getArray)
        REGISTER_TABLE_CMD(DefaultController, 0x4c, cmd_F32toF16)
        REGISTER_TABLE_CMD(DefaultController, 0x4d, cmd_F16toF32)
        REGISTER_TABLE_CMD(DefaultController, 0x4e, cmd_getElapsedTime)
};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

DefaultController::DefaultController():
    DefaultControllerBase(0x0, ::instructions, instructions_cnt)
{
    // TODO: hook the debugger to this (expected sceKernelGetSystemTimeLow)
    if (!MT_ctx_seeded) {
        MT_ctx.seed(0x0);
    }
}

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
        arg_1->as_int = cast_arg_to_int(type_2, arg_2);
    }
    else {
        arg_1->as_float = cast_arg_to_float(type_2, arg_2);
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

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    if (is_not_float_arg(type_1)) {
        arg_1->as_int += cast_arg_to_int(type_2, arg_2);
    }
    else {
        arg_1->as_float += cast_arg_to_float(type_2, arg_2);
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_sub)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    if (is_not_float_arg(type_1)) {
        arg_1->as_int -= cast_arg_to_int(type_2, arg_2);
    }
    else {
        arg_1->as_float -= cast_arg_to_float(type_2, arg_2);
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_mul)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    if (is_not_float_arg(type_1)) {
        arg_1->as_int *= cast_arg_to_int(type_2, arg_2);
    }
    else {
        arg_1->as_float *= cast_arg_to_float(type_2, arg_2);
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_div)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    if (is_not_float_arg(type_1)) {
        uint32_t operand = cast_arg_to_int(type_2, arg_2);
        if (operand == 0) {
            // TODO: call debugger
        }
        arg_1->as_int /= operand;
    }
    else {
        arg_2->as_float /= cast_arg_to_float(type_2, arg_2);
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_inc)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    auto type = ctx->get_arg_type(0);
    ctx->debug_logger(0);

    if (is_not_float_arg(type)) {
        arg->as_int += 1;
    }
    else {
        arg->as_float += 1.0;
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_dec)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    auto type = ctx->get_arg_type(0);
    ctx->debug_logger(0);

    if (is_not_float_arg(type)) {
        arg->as_int -= 1;
    }
    else {
        arg->as_float -= 1.0;
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_loop)
{
    auto iter_cnt = ctx->getArgValuePtr(0, 1, 4);
    auto dest = ctx->getArgValuePtr(1, 0, 4);

    auto current = --iter_cnt->as_int;
    if (current != 0) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_waitFrame)
{
    auto status = ctx->get_status();
    if (status == PacStatus::Zero) {
        auto frames_cnt = ctx->getArgValuePtr(0, 1, 4);
        ctx->set_frames_to_skip(frames_cnt->as_int);
        ctx->set_status(PacStatus::One);
    }
    auto frames_to_skip = ctx->get_frames_to_skip();
    if (frames_to_skip < 1) {
        ctx->set_frames_to_skip(0);
        ctx->setCmdId(0);
    }
    else {
        ctx->set_frames_to_skip(ctx->get_frames_to_skip() - 1);
    }
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_waitTime)
{
    auto status = ctx->get_status();
    if (status == PacStatus::Zero) {
        auto arg = ctx->getArgValuePtr(0, 1, 4);
        auto uVar3 = ctx->get_time_multiplier();
        float multiplier;
        if ((int)uVar3 < 0) {
            multiplier = (float)(uVar3 >> 1 | uVar3 & 1);
            multiplier = multiplier + multiplier;
        }
        else {
            multiplier = (float)uVar3;
        }
        ctx->set_remaining_time(arg->as_float * multiplier);
        ctx->set_status(PacStatus::One);
    }
    auto remaining = ctx->get_remaining_time();
    if (remaining < 1) {
        ctx->set_remaining_time(0);
        ctx->setCmdId(0);
    }
    else {
        auto delta = ctx->get_time_delta();
        ctx->set_remaining_time(ctx->get_remaining_time() - delta);
    }
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_rand)
{
    auto max = ctx->getArgValuePtr(0, 1, 4);
    auto out = ctx->getArgValuePtr(1, 1, 4);
    if (static_cast<int>(max->as_int) < 1) {
        out->as_int = 0;
    }
    else {
        auto value = MT_ctx();
        if (max->as_int == 0) {
            // TODO: call debugger
        }
        out->as_int = value % max->as_int;
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_iand)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    arg_1->as_int &= arg_2->as_int;
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ior)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    arg_1->as_int |= arg_2->as_int;
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ixor)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    arg_1->as_int ^= arg_2->as_int;
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_irol)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    arg_1->as_int <<= (arg_2->as_int & 0x1f);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_iror)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    arg_1->as_int >>= (arg_2->as_int & 0x1f);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifEQ)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int == operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float == operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifNE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int != operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float != operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLSE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int <= operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float <= operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLBE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int >= operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float >= operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLS)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int < operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float < operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifLB)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int > operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float > operand) {
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifAND)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if ((arg_1->as_int & arg_2->as_int) != 0) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifOR)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if ((arg_1->as_int | arg_2->as_int) != 0) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallEQ)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int == operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float == operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallNE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int != operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float != operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);

}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLSE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int <= operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float <= operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLBE)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int >= operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float >= operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLS)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int < operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float < operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallLB)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if (is_not_float_arg(type_1)) {
        auto operand = cast_arg_to_int(type_2, arg_2);
        if (arg_1->as_int > operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }
    else {
        auto operand = cast_arg_to_float(type_2, arg_2);
        if (arg_1->as_float > operand) {
            ctx->save_return_address();
            ctx->seek(dest->index, PacSeekMode::absolute);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallAND)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if ((arg_1->as_int & arg_2->as_int) != 0) {
        ctx->save_return_address();
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_ifCallOR)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto dest = ctx->getArgValuePtr(2, 0, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);
    ctx->debug_logger(2);

    if ((arg_1->as_int | arg_2->as_int) != 0) {
        ctx->save_return_address();
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgSet)
{
    auto flags = ctx->get_flags();
    auto args_cnt = ctx->getArgValuePtr(0, 0, 1);

    for (uint32_t i = 0; i < args_cnt->as_int; ++i) {
        auto flag_idx = ctx->getArgValuePtr(1, 0, 4);
        set_flag(flags->flags_ptr, flag_idx->as_int);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgClr)
{
    auto flags = ctx->get_flags();
    auto args_cnt = ctx->getArgValuePtr(0, 0, 1);
    for (uint32_t i = 0; i < args_cnt->as_int; ++i) {
        auto flag_idx = ctx->getArgValuePtr(1, 0, 4);
        unset_flag(flags->flags_ptr, flag_idx->as_int);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgSetId)
{
    auto flags = ctx->get_flags();
    auto flag_idx = ctx->getArgValuePtr(0, 1, 4);
    set_flag(flags->flags_ptr, flag_idx->as_int);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgClrId)
{
    auto flags = ctx->get_flags();
    auto flag_idx = ctx->getArgValuePtr(0, 1, 4);
    unset_flag(flags->flags_ptr, flag_idx->as_int);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMov)
{
    auto flags = ctx->get_flags();

    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    auto flag_value = get_flag(flags->flags_ptr, arg_2->as_int);
    if (flag_value) {
        set_flag(flags->flags_ptr, arg_1->as_int);
    }
    else {
        unset_flag(flags->flags_ptr, arg_1->as_int);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgAND)
{
    auto flags = ctx->get_flags();
    auto dest = ctx->getArgValuePtr(0, 0, 4);
    auto args_cnt = ctx->getArgValuePtr(1, 0, 1);

    bool all_set = true;
    for (uint32_t i = 0; i < args_cnt->as_int; ++i) {
        auto flag_idx = ctx->getArgValuePtr(2, 0, 4);

        auto flag_value = get_flag(flags->flags_ptr, flag_idx->as_int);
        if (!flag_value) {
            all_set = false;
            // Can't break, gotta read all args
        }
    }
    if (all_set) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgOR)
{
    auto flags = ctx->get_flags();
    auto dest = ctx->getArgValuePtr(0, 0, 4);
    auto args_cnt = ctx->getArgValuePtr(1, 0, 1);

    bool any_set = false;
    for (uint32_t i = 0; i < args_cnt->as_int; ++i) {
        auto flag_idx = ctx->getArgValuePtr(2, 0, 4);

        auto flag_value = get_flag(flags->flags_ptr, flag_idx->as_int);
        if (flag_value) {
            any_set = true;
            // Can't break, gotta read all args
        }
    }
    if (any_set) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgZERO)
{
    auto flags = ctx->get_flags();
    auto dest = ctx->getArgValuePtr(0, 0, 4);
    auto args_cnt = ctx->getArgValuePtr(1, 0, 1);

    bool none_set = true;
    for (uint32_t i = 0; i < args_cnt->as_int; ++i) {
        auto flag_idx = ctx->getArgValuePtr(2, 0, 4);

        auto flag_value = get_flag(flags->flags_ptr, flag_idx->as_int);
        if (flag_value) {
            none_set = false;
            // Can't break, gotta read all args
        }
    }
    if (none_set) {
        ctx->seek(dest->index, PacSeekMode::absolute);
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_inxJmp)
{
    auto index = ctx->getArgValuePtr(0, 1, 4);
    ctx->seek(index->as_int * sizeof(uint32_t), PacSeekMode::relative);
    auto dest = ctx->getArgValuePtr(1, 0, 4);
    ctx->seek(dest->index, PacSeekMode::absolute);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_stkDec)
{
    auto undone = ctx->get_undone_calls_count();
    if (undone == 0) {
        ctx->set_blocks_count(ctx->get_blocks_count() - 1);
    }
    else {
        ctx->undo_call_if_pending();
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_stkClr)
{
    ctx->set_blocks_count(0);
    ctx->set_undone_calls_count(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_break)
{
    char buffer[256];

    auto unk = ctx->getArgValuePtr(0, 1, 4);
    ctx->get_string_argument(buffer);

    // Using the implementation from Fate Tiger Colosseum
    Debugger::PrintLog("<<<< TALK BREAK >>>>\n");
    Debugger::PrintLog("{}\n", buffer);

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_string)
{
    char buffer[256];
    ctx->get_string_argument(buffer);
    // TODO: add the implementation from Fate Tiger Colosseum
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_print)
{
    char buffer[256];
    ctx->get_string_argument(buffer);
    // TODO: add the implementation from Fate Tiger Colosseum

    ctx->getArgValuePtr(0, 1, 4);
    auto arg = ctx->get_arg_ptr(0);
    auto type = ctx->get_arg_type(0);

    if (is_not_float_arg(type)) {

    }
    else {

    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_textOut)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    ctx->set_logging_settings(arg->as_int);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_setSleep)
{
    auto arg = ctx->getArgValuePtr(0, 1, 4);
    ctx->set_is_sleeping(arg->as_int != 0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_sinf)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto value = cast_arg_to_float(type_1, arg_1);

    arg_2->as_float = std::sinf((value * 2.0 * 3.141593) / 360.0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_cosf)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto value = cast_arg_to_float(type_1, arg_1);

    arg_2->as_float = std::cosf((value * 2.0 * 3.141593) / 360.0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_atan2f)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);
    auto arg_3 = ctx->getArgValuePtr(2, 1, 4);

    auto value = std::atan2f(arg_1->as_float, arg_2->as_float);
    arg_3->as_float = (value * 180.0) / 3.141593;
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_abs)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    auto value = cast_arg_to_float(type_1, arg_1);

    if (is_not_float_arg(type_2)) {
        arg_2->as_int = static_cast<uint32_t>(std::fabs(value));
    }
    else {
        arg_2->as_float = std::fabs(value);
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_sqrt)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    auto type_1 = ctx->get_arg_type(0);

    auto value = cast_arg_to_float(type_1, arg_1);
    arg_2->as_float = std::sqrt(value);

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_setLabelId)
{
    auto label_idx = ctx->getArgValuePtr(0, 1, 4);
    auto value = ctx->getArgValuePtr(1, 0, 4);

    ctx->set_label_value(label_idx->as_int, value->as_int);

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_callLabelId)
{
    auto label_idx = ctx->getArgValuePtr(0, 1, 4);
    ctx->debug_logger(0);
    ctx->save_return_address();

    auto dest = ctx->get_label_value(label_idx->as_int);
    ctx->seek(dest, PacSeekMode::absolute);

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_jmpLabelId)
{
    auto label_idx = ctx->getArgValuePtr(0, 1, 4);

    auto dest = ctx->get_label_value(label_idx->as_int);
    ctx->seek(dest, PacSeekMode::absolute);

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_callLabel)
{
    auto dest = ctx->getArgValuePtr(0, 1, 4);
    ctx->save_return_address();
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_jmpLabel)
{
    auto dest = ctx->getArgValuePtr(0, 1, 4);
    ctx->seek(dest->as_int, PacSeekMode::absolute);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_mod)
{
    auto arg_1 = ctx->getArgValuePtr(0, 1, 4);
    auto arg_2 = ctx->getArgValuePtr(1, 1, 4);

    ctx->debug_logger(0);
    ctx->debug_logger(1);

    auto value = arg_2->as_int;
    if (value != 0) {
        arg_1->as_int %= value;
    }

    ctx->debug_logger(0);
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_memset)
{
    auto reg_range = ctx->getArgValuePtr(0, 1, 4);
    auto value_arg = ctx->getArgValuePtr(1, 1, 4);
    auto count = ctx->getArgValuePtr(2, 1, 4);

    auto type_1 = ctx->get_arg_type(0);
    auto type_2 = ctx->get_arg_type(1);

    if (is_not_float_arg(type_1)) {
        auto value = cast_arg_to_int(type_2, value_arg);
        for (uint32_t i = 0; i < count->as_int; ++i) {
            reg_range[i].as_int = value;
        }
    }
    else {
        auto value = cast_arg_to_float(type_2, value_arg);
        for (uint32_t i = 0; i < count->as_int; ++i) {
            reg_range[i].as_float = value;
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_memcpy)
{
    auto dst = ctx->getArgValuePtr(0, 1, 4);
    auto src = ctx->getArgValuePtr(1, 1, 4);
    auto count = ctx->getArgValuePtr(2, 1, 4);

    std::memcpy(dst, src, count->as_int * sizeof(uint32_t));

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMemset)
{
    auto flags = ctx->get_flags();

    auto start = ctx->getArgValuePtr(0, 1, 4);
    auto value_arg = ctx->getArgValuePtr(1, 1, 4);
    auto count = ctx->getArgValuePtr(2, 1, 4);

    auto flag_value = value_arg->as_int == 1;
    if (flag_value) {
        for (uint32_t i = 0; i < count->as_int; ++i) {
            set_flag(flags->flags_ptr, start->as_int + i);
        }
    }
    else {
        for (uint32_t i = 0; i < count->as_int; ++i) {
            unset_flag(flags->flags_ptr, start->as_int + i);
        }
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_flgMemcpy)
{
    auto flags = ctx->get_flags();

    auto dst = ctx->getArgValuePtr(0, 1, 4);
    auto src = ctx->getArgValuePtr(1, 1, 4);
    auto count = ctx->getArgValuePtr(2, 1, 4);

    for (uint32_t i = 0; i < count->as_int; ++i) {
        auto flag_value = get_flag(flags->flags_ptr, src->as_int + i);
        if (flag_value) {
            set_flag(flags->flags_ptr, dst->as_int + i);
        }
        else {
            unset_flag(flags->flags_ptr, dst->as_int + i);
        }
    }
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_stringEnable)
{
    auto value = ctx->getArgValuePtr(0, 1, 4);
    str_enabled = value->as_int == 1;
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_srand)
{
    // TODO: hook the debugger to this (expected sceKernelGetSystemTimeLow)
    MT_ctx.seed(0x0);
    MT_ctx_seeded = true;
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_initArray)
{
    auto stack = ctx->get_stack();
    stack->reset();
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_addArray)
{
    auto offset = ctx->getArgValuePtr(0, 1, 4);
    auto out_sp = ctx->getArgValuePtr(1, 1, 4);

    auto stack = ctx->get_stack();
    auto sp = stack->push_SP(offset->as_int);

    // Sometimes float args end up here!
    out_sp->as_int = sp;

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_setArray)
{
    auto local_offset = ctx->getArgValuePtr(0, 1, 4);
    auto frame_offset = ctx->getArgValuePtr(1, 1, 4);
    auto value_arg = ctx->getArgValuePtr(2, 1, 4);

    auto type_1 = ctx->get_arg_type(1);
    auto type_2 = ctx->get_arg_type(2);

    auto stack = ctx->get_stack();

    if (is_not_float_arg(type_1)) {
        auto value = cast_arg_to_int(type_2, value_arg);
        stack->set_value_int(frame_offset->as_int, local_offset->as_int, value);
    }
    else {
        auto value = cast_arg_to_float(type_2, value_arg);
        // Yes, frame_offset is a float here, but it stores an integer... IDK why...
        stack->set_value_float(frame_offset->as_int, local_offset->as_int, value);
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_getArray)
{
    auto local_offset = ctx->getArgValuePtr(0, 1, 4);
    auto frame_offset = ctx->getArgValuePtr(1, 1, 4);
    auto out = ctx->getArgValuePtr(2, 1, 4);

    auto type_1 = ctx->get_arg_type(1);
    auto type_2 = ctx->get_arg_type(2);

    auto stack = ctx->get_stack();
    auto stack_value = stack->get_value_ptr(frame_offset->as_int, local_offset->as_int);

    if (is_not_float_arg(type_2)) {
        auto value = cast_arg_to_int(type_1, stack_value);
        out->as_int = value;
    }
    else {
        auto value = cast_arg_to_float(type_1, stack_value);
        out->as_float = value;
    }

    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_F32toF16)
{
    auto out = ctx->getArgValuePtr(0, 1, 4);
    auto value_arg = ctx->getArgValuePtr(1, 1, 4);

    uint32_t value = value_arg->as_int;
    uint32_t result = 0;

    if (value != 0) {
        result = ((value & 0x7f800000) >> 0x17) - 0x70;
        if ((int)result < 0) {
            out->as_int = 0;
            goto lab_done;
        }
        if (0x1f < (int)result) {
            result = 0x1f;
        }
        result = (value & 0x7fffff) >> 0xd | (value & 0x80000000) >> 0x10 | (result & 0x1f) << 10;
    }
    out->as_int = result;

lab_done:
    ctx->setCmdId(0);
}

COMMAND_IMPLEMENTATION(DefaultController, cmd_F16toF32)
{
    auto out = ctx->getArgValuePtr(0, 1, 4);
    auto value_arg = ctx->getArgValuePtr(1, 1, 4);

    uint16_t value;
    std::memcpy(&value, &value_arg->as_int, sizeof(uint16_t));

    uint32_t result = 0;
    if (value != 0) {
        result = (value & 0x3ff) << 0xd |
                 (value & 0x8000) << 0x10 | (((value & 0x7c00) >> 10) + 0x70) * 0x800000;
    }

    out->as_float = std::bit_cast<float>(result);
    ctx->setCmdId(0);
}
COMMAND_IMPLEMENTATION(DefaultController, cmd_getElapsedTime)
{
    auto out = ctx->getArgValuePtr(0, 1, 4);

    auto dt = ctx->get_time_delta();

    // TODO: RE this and add to the debugger
    uint32_t uVar1 = 1; // real_time_related_08875d44(1.0);
    float divisor = 0;
    if ((int)uVar1 < 0) {
        divisor = (float)(uVar1 >> 1 | uVar1 & 1);
        divisor = divisor + divisor;
    }
    else {
        divisor = (float)uVar1;
    }
    out->as_float = (float)dt / divisor;

    ctx->setCmdId(0);
}
