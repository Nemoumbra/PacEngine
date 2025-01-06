//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "base_context.h"

#include "cstring"


BasePacContext::BasePacContext(uint32_t int_var_cnt, uint32_t float_var_cnt):
        locals(int_var_cnt, float_var_cnt)
        {
            ctl_ptr = controllers[0];
            ctl_index = 0;
        }


RegisterGroup *BasePacContext::get_locals() {
    return &locals;
}

uint32_t BasePacContext::run(uint32_t dt) {
    blocks[cur_block].cmd_waitTime_dt = dt;
    // TODO: pac reader call
}

BasePacContext::~BasePacContext() {
    // probably nothing important for us?
}

void BasePacContext::reset() {
    std::memset(blocks, 0, sizeof(blocks));
    logging_mask = 0;
    sleeping = false;
    cur_block = 0;
    stack_top = blocks[0].stack + blocks[0].undone_calls_cnt;
    cmd_waitFrame_multiplier = 0;
    global_runtime = nullptr;
    unknown1 = 0;
}

void BasePacContext::clear() {
    // delete the controllers
    ctl_index = 0;
}

void BasePacContext::set_runtime(PacGlobalRuntime *runtime) {
    global_runtime = runtime;
}

bool BasePacContext::set_is_sleeping(bool is_sleeping) {
    bool old_value = this->sleeping;
    sleeping = is_sleeping;
    return old_value;
}
