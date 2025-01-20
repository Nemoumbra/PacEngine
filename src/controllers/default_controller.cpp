//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller.h"
#include "../context/base_context.h"

#include "../debugger/debugger.h"

#include <random>


static std::mt19937 MT_ctx;
static bool MT_ctx_seeded = false;


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
