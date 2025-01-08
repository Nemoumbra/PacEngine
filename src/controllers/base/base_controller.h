//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>

// Forward declaration
struct BasePacContext;

#define REGISTER_COMMAND(command_name) void command_name(BasePacContext* ctx, int sec_id, int instr_id)
#define REGISTER_TABLE_CMD(ctl, id, cmd_name) {id, static_cast<Handler>(&ctl::cmd_name), #cmd_name},
#define REGISTER_TABLE_BEGIN {0x0, nullptr, "null"},
#define LOG_ME Debugger::PrintLog("{}\n", this->instructions[instr_id].name);


struct BaseController {
    virtual ~BaseController() = default;
    virtual bool execute(BasePacContext* ctx, uint32_t sec_id, uint32_t cmd_id, uint32_t dt) = 0;
    virtual int get_something() = 0;
};