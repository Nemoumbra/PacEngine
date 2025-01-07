//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "base_controller.h"
#include "../../instruction.h"

struct DefaultControllerBase: BaseController {
    uint32_t section_id;
    Instruction* instructions;
    uint32_t instr_cnt;

    DefaultControllerBase(uint32_t sec_id);
    virtual ~DefaultControllerBase() override = default;
    bool execute(BasePacContext *ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) override;
    int get_something() override;
    virtual bool is_right_sec_id(uint32_t sec_id);
    virtual void get_handler(Handler* out, uint32_t index);
};