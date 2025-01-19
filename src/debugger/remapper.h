//
// Created by Nemoumbra on 19.01.2025.
//

#pragma once
#include <cstdint>

#include <map>

template <typename AddrType>
struct MemoryRange {
    AddrType start;
    uint32_t size;
};


class Remapper {
private:
    std::map<uintptr_t, MemoryRange<uint32_t>> real_to_virtual;
    std::map<uint32_t, MemoryRange<uintptr_t>> virtual_to_real;

    template<typename T, typename U>
    bool add_to_map(std::map<T, MemoryRange<U>>& map, T address, uint32_t size, U virtual_address) {
        if (map.empty()) {
            map.emplace(address, MemoryRange<U>{virtual_address, size});
            return true;
        }
        // Not empty
        auto first_gt = map.upper_bound(address);

        if (first_gt == map.begin()) {
            // Before the first range => we either intersect the first range or fully lie before it
            const auto& [start, _] = *first_gt;
            if (address + size >= start) {
                // Can't add it
                return false;
            }
            // We can add it!
            map.emplace(address,  MemoryRange<U>{virtual_address, size});
            return true;
        }

        const auto& [prev_start, prev_range] = *(std::prev(first_gt, 1));
        // Does the address lie inside the previous range?
        if (address < prev_start + prev_range.size) {
            // Yes, it does...
            return false;
        }
        const auto& [start, _] = *first_gt;
        if (address + size >= start) {
            // Can't add it
            return false;
        }

        map.emplace(address,  MemoryRange<U>{virtual_address, size});
        return true;
    }

    template <typename T, typename U>
    bool do_remap(std::map<T, MemoryRange<U>>& map, T input_address, U* out_address) {
        if (map.empty()) {
            return false;
        }
        // Not empty
        auto first_gt = map.upper_bound(input_address);

        if (first_gt == map.begin()) {
            // Cannot remap (the address is before the first range)
            return false;
        }

        const auto& [prev_start, prev_range] = *(std::prev(first_gt, 1));
        if (input_address < prev_start + prev_range.size) {
            // The address lies inside the previous range
            auto diff = input_address - prev_start;
            *out_address = prev_range.start + diff;
            return true;
        }

        // Then it's not in any of the ranges!
        return false;
    }

public:
    bool RegisterFile(uintptr_t start, uint32_t size, uint32_t virtual_start);
    template<typename T>
    bool RegisterFile(const T* start, uint32_t size, uint32_t virtual_start) {
        return RegisterFile(static_cast<uintptr_t>(start), size, virtual_start);
    }

    bool RemapAddress(uint32_t address, uintptr_t* remapped);
    template<typename T>
    bool RemapAddress(uint32_t address, T** remapped) {
        return RemapAddress(address, reinterpret_cast<uintptr_t*>(remapped));
    }

    bool RemapAddress(uintptr_t address, uint32_t* remapped);
    template<typename T>
    bool RemapAddress(const T* address, uint32_t* remapped) {
        return RemapAddress(static_cast<uintptr_t>(address), remapped);
    }
};
