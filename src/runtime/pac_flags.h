//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include "cstdint"

struct Flags {
    char* flags_ptr;
    uint32_t capacity;

    Flags(uint32_t capacity);
    ~Flags();
};
