//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

#include "../context/pac_types.h"


struct Stack {
    uint32_t* stack_ptr;
    uint32_t capacity;
    uint32_t PAC_SP;

    Stack(uint32_t capacity);
    ~Stack();

    uint32_t push_SP(int32_t offset);
    PacArgValue* get_value_ptr(int32_t frame_offset, int32_t local_offset);
    uint32_t set_value_int(int frame_offset, int local_offset, uint32_t value);
    float set_value_float(int frame_offset, int local_offset, float value);

    uint32_t get_size_in_bytes();
    void reset();
};
