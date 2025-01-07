//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

// Forward declaration
struct BasePacContext;

#define REGISTER_COMMAND(command_name) void command_name(PacContext* ctx, int sec_id, int instr_id)

struct BaseController {
    virtual ~BaseController() = default;
    virtual bool execute(BasePacContext* ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) = 0;
    virtual int get_something() = 0;
};