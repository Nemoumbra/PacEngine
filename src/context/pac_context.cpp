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