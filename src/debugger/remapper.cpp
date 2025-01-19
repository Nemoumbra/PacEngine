//
// Created by Nemoumbra on 19.01.2025.
//

#include "remapper.h"


bool Remapper::RemapAddress(uint32_t address, uintptr_t* remapped) {
    return do_remap(virtual_to_real, address, remapped);
}

bool Remapper::RemapAddress(uintptr_t address, uint32_t* remapped) {
    return do_remap(real_to_virtual, address, remapped);
}

bool Remapper::RegisterFile(uintptr_t start, uint32_t size, uint32_t virtual_start) {
    if (!add_to_map(real_to_virtual, start, size, virtual_start)) {
        return false;
    }
    if (!add_to_map(virtual_to_real, virtual_start, size, start)) {
        return false;
    }
    return true;
}
