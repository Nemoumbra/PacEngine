//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

struct Flags {
    char* flags_ptr;
    uint32_t capacity;

    Flags(uint32_t capacity);
    ~Flags();
};

constexpr bool get_flag(char* flags_ptr, uint32_t index) {
    auto byte_idx = index / 8;
    auto bit_idx = index % 8;

    return flags_ptr[byte_idx] | (1 << bit_idx);
}

constexpr void set_flag(char* flags_ptr, uint32_t index) {
    auto byte_idx = index / 8;
    auto bit_idx = index % 8;

    auto mask = static_cast<char>(1 << bit_idx);

    flags_ptr[byte_idx] |= mask;
}

constexpr void unset_flag(char* flags_ptr, uint32_t index) {
    auto byte_idx = index / 8;
    auto bit_idx = index % 8;

    auto mask = static_cast<char>(1 << bit_idx) ^ 0xff;
    flags_ptr[byte_idx] &= mask;
}