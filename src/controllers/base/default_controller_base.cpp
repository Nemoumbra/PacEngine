//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller_base.h"
#include "../../context/base_context.h"


DefaultControllerBase::DefaultControllerBase(uint32_t sec_id, Instruction* table_ptr, uint32_t table_elem_cnt):
    section_id(sec_id),
    instructions(table_ptr),
    instr_cnt(table_elem_cnt) {}

bool DefaultControllerBase::execute(BasePacContext *ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) {
    if (!is_right_sec_id(sec_id)) {
        return false;
    }
    ctx->set_time_delta(dt);

    Handler handler;
    get_handler(&handler, cmd_id);
    (this->*handler)(ctx, sec_id, cmd_id);
    return true;
}

int DefaultControllerBase::get_something() {
    return section_id;
}

bool DefaultControllerBase::is_right_sec_id(uint32_t sec_id) {
    return section_id == sec_id;
}

void DefaultControllerBase::get_handler(Handler *out, uint32_t index) {
    Instruction* current = instructions;
    Instruction* end = instructions + instr_cnt;
    for (; current != end; ++current) {
        if (current->id == index) {
            *out = current->handler;
            return;
        }
    }
}
