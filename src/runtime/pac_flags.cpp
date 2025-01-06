//
// Created by Nemoumbra on 06.01.2025.
//

#include "pac_flags.h"

Flags::Flags(uint32_t capacity):
        flags_ptr(nullptr),
        capacity(capacity)
{
    if (capacity) {
        flags_ptr = new char[capacity];
    }
}
Flags::~Flags() {
    delete[] flags_ptr;
}
