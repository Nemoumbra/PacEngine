//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once
#include <cstdint>
#include "../controllers/base/base_controller.h"
#include "../runtime/pac_runtime.h"
#include "pac_types.h"


class BasePacContext {
protected:
    uint32_t unknown1;
    PacPosition* stack_top;
    bool sleeping;
    uint32_t logging_mask;
    uint32_t cmd_waitFrame_multiplier;
    PacBlock blocks[3];
    uint32_t cur_block;

    BaseController* controllers[20];
    uint32_t unknown2;

    BaseController** ctl_wrap_ptr;
    uint32_t ctl_wrap_index;

    PacGlobalRuntime* global_runtime;

    RegisterGroup locals;

public:
    BasePacContext(uint32_t int_var_cnt, uint32_t float_var_cnt);
    virtual ~BasePacContext();
    virtual RegisterGroup* get_locals();
    virtual uint32_t run(uint32_t dt);

    void reset();
    void clear();
    void set_runtime(PacGlobalRuntime* runtime);

    bool set_is_sleeping(bool is_sleeping);
    uint32_t get_time_multiplier();
    uint32_t get_time_delta();
    void set_time_delta(uint32_t dt);
    void* get_pac_start();
    void set_pac_start(char* start);
    void* get_pc();
    void set_starting_pc(char* start);
    void set_label_value(uint32_t index, uint32_t value);
    uint32_t get_label_value(uint32_t index);
    void* seek(uint32_t offset, PacSeekMode mode);
    void save_return_address();
    void undo_call();
    uint32_t get_undone_calls_count();
    void set_undone_calls_count(uint32_t value);
    void undo_call_if_pending();
    uint32_t get_logging_settings();
    void set_logging_settings(uint32_t value);
    PacStatus get_status();
    void set_status(PacStatus status);
    void setCmdId(uint32_t id);
    uint32_t get_frames_to_skip();
    void set_frames_to_skip(uint32_t value);
    uint32_t get_remaining_time();
    void set_remaining_time(uint32_t value);
    bool get_result();
    void set_result(bool value);
    Flags* get_flags();
    Stack* get_stack();
    uint32_t interpret(uint32_t dt, uint32_t setting);
    uint32_t register_controller(BaseController* ctl);
    void unregister_controller();
    PacArgType get_arg_type(uint32_t arg_index);
    PacArgValue get_arg_value(uint32_t arg_index);
    PacArgValue* get_arg_ptr(uint32_t arg_index);
    PacArgValue* getArgValuePtr(uint32_t arg_index, uint32_t not_immediate, uint32_t size);
    void debug_logger(uint32_t arg_index);
    void get_string_argument(char* out);
    void make_new_block();
    void revert_to_previous_block();
    uint32_t get_blocks_count();
    uint32_t set_blocks_count(uint32_t value);
    void push_pc(uint32_t alignment);
};
