//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "../base/custom_controller_base.h"

struct ScriptControllerBase : CustomControllerBase {
    uint32_t section_id;
    Instruction* instructions;
    uint32_t instr_cnt;

    ScriptControllerBase(uint32_t sec_id, Instruction* table_ptr, uint32_t table_elem_cnt);

    ~ScriptControllerBase() override = default;
    bool execute(BasePacContext *ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) override;
    virtual bool is_right_sec_id(uint32_t sec_id);
    virtual void get_handler(Handler* out, uint32_t index);
};