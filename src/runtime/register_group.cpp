//
// Created by Nemoumbra on 06.01.2025.
//

#include "register_group.h"

RegisterGroup::RegisterGroup(uint32_t int_reg_cnt, uint32_t float_reg_cnt):
        int_regs_count(int_reg_cnt),
        int_regs(nullptr),
        float_regs_count(float_reg_cnt),
        float_regs(nullptr)
{
    if (int_regs_count) {
        int_regs = new int32_t[int_regs_count];
    }
    if (float_regs_count) {
        float_regs = new float[float_regs_count];
    }
}

RegisterGroup::~RegisterGroup() {
    delete[] int_regs;
    delete[] float_regs;
}

int32_t* RegisterGroup::get_int_reg_ptr(int32_t index) {
    return &int_regs[index];
}

float * RegisterGroup::get_float_reg_ptr(int32_t index) {
    return &float_regs[index];
}
int32_t RegisterGroup::get_int_reg(int32_t index) {
    return int_regs[index];
}

float RegisterGroup::get_float_reg(int32_t index) {
    return float_regs[index];
}

int32_t RegisterGroup::set_int_reg(int32_t index, int32_t value) {
    int_regs[index] = value;
    return value;
}

float RegisterGroup::set_float_reg(int32_t index, float value) {
    float_regs[index] = value;
    return value;
}

void RegisterGroup::int_reg_memset(int32_t value) {
    for (int i = 0; i < int_regs_count; ++i) {
        int_regs[i] = value;
    }
}

void RegisterGroup::float_reg_memset(float value) {
    for (int i = 0; i < float_regs_count; ++i) {
        float_regs[i] = value;
    }
}

void RegisterGroup::save_regs(int32_t* out_int_buffer, float* out_float_buffer) {
    for (int i = 0; i < int_regs_count; ++i) {
        out_int_buffer[i] = int_regs[i];
    }
    for (int i = 0; i < float_regs_count; ++i) {
        out_float_buffer[i] = float_regs[i];
    }
}

void RegisterGroup::restore_regs(int32_t *int_buffer, float *float_buffer) {
    for (int i = 0; i < int_regs_count; ++i) {
        int_regs[i] = int_buffer[i];
    }
    for (int i = 0; i < float_regs_count; ++i) {
        float_regs[i] = float_buffer[i];
    }
}
