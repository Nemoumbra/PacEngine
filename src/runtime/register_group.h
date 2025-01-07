//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>
#include "../context/pac_types.h"

struct RegisterGroup {
    int32_t* int_regs;
    uint32_t int_regs_count;

    float* float_regs;
    uint32_t float_regs_count;

    RegisterGroup(uint32_t int_reg_cnt, uint32_t float_reg_cnt);
    ~RegisterGroup();

    int32_t* get_int_reg_ptr(int32_t index);
    float* get_float_reg_ptr(int32_t index);

    int32_t get_int_reg(int32_t index);
    float get_float_reg(int32_t index);

    int32_t set_int_reg(int32_t index, int32_t value);
    float set_float_reg(int32_t index, float value);

    void int_reg_memset(int32_t value);
    void float_reg_memset(float value);

    void save_regs(int32_t* out_int_buffer, float* out_float_buffer);
    void restore_regs(int32_t* int_buffer, float* float_buffer);
};