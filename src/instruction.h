//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include "controllers/base/base_controller.h"

using Handler = void (BaseController::*)(PacContext* ctx, int sec_id, int instr_id);

struct Instruction {
    int id;
    Handler handler;
    const char* name;
};
