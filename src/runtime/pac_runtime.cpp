//
// Created by Nemoumbra on 06.01.2025.
//

#include "pac_runtime.h"


PacGlobalRuntime::PacGlobalRuntime(int32_t int_reg_cnt, int32_t float_reg_cnt):
    flags(0x400),
    globals(int_reg_cnt, float_reg_cnt),
    stack(0x100),
    labels(0x100)
    {}

Flags* PacGlobalRuntime::get_flags() {
    return &flags;
}

Stack* PacGlobalRuntime::get_stack() {
    return &stack;
}

RegisterGroup* PacGlobalRuntime::get_globals() {
    return &globals;
}

Labels* PacGlobalRuntime::get_labels() {
    return &labels;
}
