//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include "controllers/base/base_controller.h"

// class PacContext;

using Handler = void (BaseController::*)(BasePacContext* ctx, int sec_id, int instr_id);

struct Instruction {
    int id;
    Handler handler;
    const char* name;
};
