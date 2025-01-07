//
// Created by Nemoumbra on 06.01.2025.
//

#include "pac_context.h"

PacContext::PacContext(uint32_t int_reg_cnt, uint32_t float_reg_cnt, uint32_t make_0x10_section):
        DerivedPacContext(int_reg_cnt, float_reg_cnt),
        unknown3(0),
        script_manager(nullptr),
        parent_ctx(nullptr),
        script_label_table(nullptr),
        ctl(nullptr),
        call_c7c(false),
        unk(false),
        call_reader(true),
        group_id(0)
{
    // set_runtime(???);
    // We don't have a singleton global runtime here, do we?

    if (make_0x10_section) {
        // TODO: register section 0x10 (not into ctl, but into the array, I guess)
    }
}

PacContext::~PacContext()  {
    // probably nothing important for us?
    // just delete the ctl
}

uint32_t PacContext::run(uint32_t dt) {
    if (call_reader) {
        return BasePacContext::run(dt);
    }
    return 1;
}

uint32_t PacContext::run_script(uint32_t offset, uint32_t dt, char* param_4) {
    auto file_offset = get_pac_start();
    get_pc();
    get_undone_calls_count();
    get_blocks_count();
    make_new_block();
    if (param_4 == 0) {
        set_starting_pc(file_offset);
    } else {
        set_starting_pc(param_4);
    }

    seek(offset, PacSeekMode::absolute);
    interpret(dt, 1);
    get_pac_start();
    get_pc();
    get_undone_calls_count();
    get_blocks_count();
    return 1;
}

uint32_t PacContext::set_script_label_table(uint32_t offset) {
    auto PAC_start = get_pac_start();
    if (PAC_start) {
        script_label_table = PAC_start + offset;
    }
    return 0;
}

uint32_t PacContext::set_script_argument(uint32_t value, uint32_t arg_index) {
    RegisterGroup* local_regs = get_locals();
    local_regs->set_int_reg(0x40 + arg_index, value);
}

PacContext *PacContext::get_root_context() {
    auto ctx = parent_ctx;
    if (!ctx) {
        return this;
    }

    for (; ctx->parent_ctx != nullptr; ctx = ctx->parent_ctx) {

    }
    return ctx;
}