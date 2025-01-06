//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "pac_flags.h"
#include "register_group.h"
#include "pac_stack.h"
#include "pac_labels.h"

#include <cstdint>

class PacGlobalRuntime {
private:
    Flags flags;
    RegisterGroup globals;
    Stack stack;
    Labels labels;

public:
    Flags* get_flags();
    RegisterGroup* get_globals();
    Stack* get_stack();
    Labels* get_labels();

    PacGlobalRuntime(int32_t int_reg_cnt, int32_t float_reg_cnt);
};
