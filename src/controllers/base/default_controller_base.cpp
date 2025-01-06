//
// Created by Nemoumbra on 06.01.2025.
//

#include "default_controller_base.h"
#include "../../instruction.h"

static const char* names[] = {
        "null",
        "cmd_end",
        "cmd_call",
        "cmd_jmp"
};

constexpr uint32_t instructions_cnt = sizeof(names) / sizeof(names[0]);
static Instruction instructions[instructions_cnt] = {};

DefaultControllerBase::DefaultControllerBase(uint32_t sec_id):
    section_id(sec_id),
    instructions(::instructions),
    instr_cnt(instructions_cnt) {}

bool DefaultControllerBase::execute(PacContext *ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) {
    if (!is_right_sec_id(sec_id)) {
        return false;
    }
    // TODO: PAC_setTimeDelta
    Handler handler;
    get_handler(&handler, cmd_id);
    (this->*handler)(ctx, sec_id, cmd_id);
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
            out = &current->handler;
            return;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//////////////////// The implementation of the Instructions ////////////////////
////////////////////////////////////////////////////////////////////////////////

