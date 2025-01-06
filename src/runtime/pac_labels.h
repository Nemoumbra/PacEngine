//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>


struct Labels {
    uint32_t labels[256];
    uint32_t padding;
    uint32_t* labels_ptr;
    uint32_t capacity;

    static constexpr uint32_t default_value = 0x0;

    Labels(uint32_t capacity);
    ~Labels() = default;
};
