//
// Created by Nemoumbra on 06.01.2025.
//

#include "pac_labels.h"

Labels::Labels(uint32_t capacity):
    labels_ptr(labels),
    capacity(capacity)
{
    for (uint32_t* ptr = labels_ptr; ptr != labels_ptr + capacity; ++ptr) {
        *ptr = default_value;
    }
}