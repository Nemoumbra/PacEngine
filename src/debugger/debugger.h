//
// Created by Nemoumbra on 08.01.2025.
//

#pragma once
#include <cstdint>
#include <format>
#include <iostream>


struct Debugger {
    static void OnUnknownSection(uint32_t sec_id);
    static void OnUnknownInstruction(uint32_t sec_id, uint32_t cmd_id);
    static void OnCmdEnd();

    template<typename... Args>
    static void PrintLog(const std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::vformat(fmt.get(), std::make_format_args(args...));
    }
};
