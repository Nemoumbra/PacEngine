//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "derived_context.h"


class PacContext: public DerivedPacContext {
protected:
    uint32_t unknown3;
    void* script_manager;
    PacContext* parent_ctx;
    void* script_label_table;

    BaseController* ctl;
    bool call_c7c;
    bool unk;
    bool call_reader;

    uint32_t group_id;

public:
    PacContext(uint32_t int_reg_cnt, uint32_t float_reg_cnt, uint32_t make_0x10_section);
    ~PacContext() override;
    uint32_t run(uint32_t dt) override;

    uint32_t run_script(uint32_t offset, uint32_t dt, char* param_4); // PAC_calls_PAC_reader_08909b58
    uint32_t set_script_label_table(uint32_t offset);
    uint32_t set_script_argument(uint32_t value, uint32_t arg_index);
    PacContext* get_root_context();
};