//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

enum PacArgType : uint8_t {
    None = 0,
    Index = 1,
    IntImm = 2,
    IntLocal = 4,
    IntGlobal = 8,
    FloatImm = 16,
    FloatLocal = 32,
    FloatGlobal = 64
};

constexpr bool is_not_float_arg(PacArgType type) {
    return (type & (FloatImm | FloatLocal | FloatGlobal)) == 0;
}

constexpr bool is_int_reg(PacArgType type) {
    return (type & (IntLocal | IntGlobal)) != 0;
}

constexpr bool is_local_int_reg(PacArgType type) {
    return (type & IntLocal) != 0;
}

constexpr bool is_local_float_reg(PacArgType type) {
    return (type & FloatLocal) != 0;
}

union PacArgValue {
    uint32_t as_int;
    float as_float;
    uint32_t index;
};

constexpr uint32_t cast_arg_to_int(PacArgType arg_type, PacArgValue* arg) {
    return is_not_float_arg(arg_type) ? arg->as_int : static_cast<uint32_t>(arg->as_float);
}

constexpr float cast_arg_to_float(PacArgType arg_type, PacArgValue* arg) {
    return is_not_float_arg(arg_type) ? static_cast<float>(arg->as_int) : arg->as_float;
}

struct PacArgument {
    PacArgType type;
    PacArgValue value;
};

struct PacPosition {
    char* PAC_start;
    char* PAC_PC;
};

enum PacStatus : uint32_t {
    Zero = 0,
    One = 1,
    Two = 2
};

enum class PacSeekMode {
    absolute = 0,
    relative = 1
};

struct PacBlock {
    PacPosition stack[15];
    uint32_t undone_calls_cnt;
    uint32_t cmd_waitTime_dt;
    PacArgument args[16];
    PacStatus status;
    uint32_t sec_id;
    uint32_t cmd_id;
    uint32_t frames_to_skip;
    uint32_t cmd_waitTime_buffer;
    bool result;
};
