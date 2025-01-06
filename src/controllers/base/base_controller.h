//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

// Forward declaration
struct PacContext;

struct BaseController {
    virtual ~BaseController() = default;
    virtual bool execute(PacContext* ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) = 0;
    virtual int get_something() = 0;
};