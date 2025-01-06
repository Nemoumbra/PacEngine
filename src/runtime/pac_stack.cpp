//
// Created by Nemoumbra on 06.01.2025.
//

#include "pac_stack.h"

#include <bit>
#include <cstring>

Stack::Stack(uint32_t capacity):
        stack_ptr(nullptr),
        capacity(capacity),
        PAC_SP(0)
{
    if (capacity) {
        stack_ptr = new uint32_t[capacity];
    }
}

Stack::~Stack() {
    delete[] stack_ptr;
}

void Stack::push_SP(int32_t offset) {
    PAC_SP += offset;
}

uint32_t* Stack::get_value_ptr(int32_t frame_offset, int32_t local_offset) {
    return &stack_ptr[frame_offset + local_offset];
}

uint32_t Stack::set_value_int(int frame_offset, int local_offset, uint32_t value) {
    stack_ptr[frame_offset + local_offset] = value;
    return value;
}
uint32_t Stack::set_value_float(int frame_offset, int local_offset, float value) {
    stack_ptr[frame_offset + local_offset] = std::bit_cast<uint32_t, float>(value);
    return value;
}

uint32_t Stack::get_size_in_bytes() {
    return capacity * sizeof(uint32_t);
}

void Stack::reset() {
    std::memset(stack_ptr, 0, get_size_in_bytes());
    PAC_SP = 0;
}
