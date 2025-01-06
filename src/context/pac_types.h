//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

enum PacArgType : uint8_t {
    None=0,
    Index=1,
    IntImm=2,
    IntLocal=4,
    IntGlobal=8,
    FloatImm=16,
    FloatLocal=32,
    FloatGlobal=64
};

union PacArgValue {
    uint32_t as_int;
    float as_float;
    uint32_t index;
};

struct PacArgument {
    PacArgType type;
    PacArgValue value;
};

struct PacPosition {
    void* PAC_start;
    void* PAC_PC;
};

enum PacStatus : uint32_t {
    Zero=0,
    One=1,
    Two=2
};

enum class PacSeekMode {
    absolute,
    relative
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
