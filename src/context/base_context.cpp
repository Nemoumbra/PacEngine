//
// Created by Nemoumbra on 06.01.2025.
//

#include "base_context.h"

#include "cstring"


BasePacContext::BasePacContext(uint32_t int_var_cnt, uint32_t float_var_cnt):
        locals(int_var_cnt, float_var_cnt)
        {
            ctl_wrap_ptr = &controllers[0];
            ctl_wrap_index = 0;
            reset();
        }


RegisterGroup *BasePacContext::get_locals() {
    return &locals;
}

uint32_t BasePacContext::run(uint32_t dt) {
    blocks[cur_block].cmd_waitTime_dt = dt;
    return interpret(dt, 0);
}

BasePacContext::~BasePacContext() {
    // probably nothing important for us?
}

void BasePacContext::reset() {
    std::memset(blocks, 0, sizeof(blocks));
    logging_mask = 0;
    sleeping = false;
    cur_block = 0;
    stack_top = blocks[0].stack + blocks[0].undone_calls_cnt;
    cmd_waitFrame_multiplier = 0;
    global_runtime = nullptr;
    unknown1 = 0;
}

void BasePacContext::clear() {
    // delete the controllers
    ctl_wrap_index = 0;
}

void BasePacContext::set_runtime(PacGlobalRuntime *runtime) {
    global_runtime = runtime;
}

bool BasePacContext::set_is_sleeping(bool is_sleeping) {
    bool old_value = this->sleeping;
    sleeping = is_sleeping;
    return old_value;
}

uint32_t BasePacContext::get_time_multiplier() {
    return cmd_waitFrame_multiplier;
}

uint32_t BasePacContext::get_time_delta() {
    return blocks[cur_block].cmd_waitTime_dt;
}

void BasePacContext::set_time_delta(uint32_t dt) {
    blocks[cur_block].cmd_waitTime_dt = dt;
}

char *BasePacContext::get_pac_start() {
    return stack_top->PAC_start;
}

void BasePacContext::set_pac_start(char *start) {
    stack_top->PAC_start = start;
}

char *BasePacContext::get_pc() {
    return stack_top->PAC_PC;
}

void BasePacContext::set_starting_pc(char *start) {
    stack_top->PAC_start = start;
    stack_top->PAC_PC = start;
}

void BasePacContext::set_label_value(uint32_t index, uint32_t value) {
    Labels* labels = global_runtime->get_labels();
    labels->labels_ptr[index] = value;
}

uint32_t BasePacContext::get_label_value(uint32_t index) {
    Labels* labels = global_runtime->get_labels();
    return labels->labels_ptr[index];
}

char *BasePacContext::seek(uint32_t offset, PacSeekMode mode) {
    uint32_t mask = get_logging_settings();
    if (mask & 4) {
        // TODO: log
    }
    char* ptr = nullptr;

    if (mode == PacSeekMode::relative) {
        ptr = stack_top->PAC_PC;
    }
    else {
        ptr = stack_top->PAC_start;
    }
    ptr += offset;
    stack_top->PAC_PC = ptr;

    return ptr;
}

void BasePacContext::save_return_address() {
    auto undone = get_undone_calls_count();
    blocks[cur_block].stack[undone].PAC_start = stack_top->PAC_start;
    blocks[cur_block].stack[undone].PAC_PC = stack_top->PAC_PC;

    blocks[cur_block].stack[undone + 1].PAC_start = stack_top->PAC_start;
    blocks[cur_block].stack[undone + 1].PAC_PC = stack_top->PAC_PC;

    stack_top = blocks[cur_block].stack + undone + 1;
    set_undone_calls_count(undone + 1);
}

void BasePacContext::undo_call() {
    auto undone = get_undone_calls_count();
    stack_top = blocks[cur_block].stack + undone - 1;
    set_undone_calls_count(undone - 1);
}

uint32_t BasePacContext::get_undone_calls_count() {
    return blocks[cur_block].undone_calls_cnt;
}

void BasePacContext::set_undone_calls_count(uint32_t value) {
    blocks[cur_block].undone_calls_cnt = value;
}

void BasePacContext::undo_call_if_pending() {
    auto& count = blocks[cur_block].undone_calls_cnt;
    if (count) {
        --count;
    }
}

uint32_t BasePacContext::get_logging_settings() {
    return logging_mask;
}

void BasePacContext::set_logging_settings(uint32_t value) {
    logging_mask = value;
}

PacStatus BasePacContext::get_status() {
    return blocks[cur_block].status;
}

void BasePacContext::set_status(PacStatus status) {
    blocks[cur_block].status = status;
}

void BasePacContext::setCmdId(uint32_t id) {
    blocks[cur_block].cmd_id = id;
}

uint32_t BasePacContext::get_frames_to_skip() {
    return blocks[cur_block].frames_to_skip;
}

void BasePacContext::set_frames_to_skip(uint32_t value) {
    blocks[cur_block].frames_to_skip = value;
}

uint32_t BasePacContext::get_remaining_time() {
    return blocks[cur_block].cmd_waitTime_buffer;
}

void BasePacContext::set_remaining_time(uint32_t value) {
    blocks[cur_block].cmd_waitTime_buffer = value;
}

bool BasePacContext::get_result() {
    return blocks[cur_block].result;
}

void BasePacContext::set_result(bool value) {
    blocks[cur_block].result = value;
}

Flags *BasePacContext::get_flags() {
    return global_runtime->get_flags();
}

Stack *BasePacContext::get_stack() {
    return global_runtime->get_stack();
}

uint32_t BasePacContext::interpret(uint32_t dt, uint32_t setting) {
    if (!stack_top->PAC_PC || !stack_top->PAC_start) {
        return 0;
    }
    if ((setting & 0xFF) == 0) {
        if (sleeping) {
            return 0;
        }
    }

    auto undone = blocks[cur_block].undone_calls_cnt;
    auto blocks_cnt = get_blocks_count();

    auto cur_block_idx = cur_block;
    auto ctl_idx_ptr = &ctl_wrap_index;
    PacBlock* block_ptr = nullptr;

lab1:
    cur_block_idx = cur_block;
    do {
        block_ptr = blocks + cur_block_idx;
        if (blocks[cur_block_idx].cmd_id == 0) {
            blocks[cur_block_idx].status = PacStatus::Zero;
            auto& pc = stack_top->PAC_PC;
            if (*pc != '%') {
                ++pc;
                goto lab1;
            }
            blocks[cur_block_idx].sec_id = pc[1];
            auto cmd_ptr = reinterpret_cast<short*>(pc + 2);
            blocks[cur_block_idx].cmd_id = *cmd_ptr;

            pc += 4;
        }

        auto ctl_idx = *ctl_idx_ptr;
        auto controller_wrap_ptr = ctl_wrap_ptr;
        PacPosition* pos_ptr = nullptr;

        if (controller_wrap_ptr != ctl_wrap_ptr + ctl_idx) {
            auto controller = *controller_wrap_ptr;
            while (!controller->execute(this, blocks[cur_block_idx].sec_id, blocks[cur_block_idx].cmd_id, dt)) {
                if (controller_wrap_ptr + 1 == ctl_wrap_ptr + *ctl_idx_ptr) {
                    goto lab2;
                }
                controller = controller_wrap_ptr[1];
                ++controller_wrap_ptr;
            }
            block_ptr = blocks + cur_block;
            lab2:
            pos_ptr = stack_top; // Have to keep it here for lab2:
        } else {
            pos_ptr = stack_top;
        }
        if (pos_ptr->PAC_start == nullptr) {
            return 1;
        }
        if (setting == 0) {
            if (sleeping) {
                return 1;
            }
            if (block_ptr->cmd_id != 0) {
                return 1;
            }
            goto lab1;
        }
        auto current_blocks_cnt = get_blocks_count();
        if (current_blocks_cnt < blocks_cnt) {
            return 1;
        }
        if (current_blocks_cnt == blocks_cnt) {
            break;
        }
        cur_block_idx = cur_block;
    } while (true);

    if (blocks[cur_block].undone_calls_cnt < undone) {
        return 1;
    }
    goto lab1;
}

uint32_t BasePacContext::register_controller(BaseController *ctl) {
    ctl_wrap_ptr[ctl_wrap_index] = ctl;
    ++ctl_wrap_index;
    return 1;
}

void BasePacContext::unregister_controller() {
    // Ignore?
}

PacArgType BasePacContext::get_arg_type(uint32_t arg_index) {
    return blocks[cur_block].args[arg_index].type;
}

PacArgValue BasePacContext::get_arg_value(uint32_t arg_index) {
    return blocks[cur_block].args[arg_index].value;
}

PacArgValue *BasePacContext::get_arg_ptr(uint32_t arg_index) {
    RegisterGroup* local_regs = get_locals();
    RegisterGroup* global_regs = global_runtime->get_globals();
    auto type = blocks[cur_block].args[arg_index].type;
    switch (type) {
        case FloatGlobal:
            return reinterpret_cast<PacArgValue*>(global_regs->get_float_reg_ptr(arg_index));
        case FloatLocal:
            return reinterpret_cast<PacArgValue*>(local_regs->get_float_reg_ptr(arg_index));
        case FloatImm:
            return &blocks[cur_block].args[arg_index].value;
        case IntGlobal:
            return reinterpret_cast<PacArgValue*>(global_regs->get_int_reg_ptr(arg_index));
        case IntLocal:
            return reinterpret_cast<PacArgValue*>(local_regs->get_int_reg_ptr(arg_index));
        case IntImm:
            return &blocks[cur_block].args[arg_index].value;
        case Index:
            return &blocks[cur_block].args[arg_index].value;
        case None:
            return &blocks[cur_block].args[arg_index].value;
        default:
            return nullptr;
    }
}

PacArgValue *BasePacContext::getArgValuePtr(uint32_t arg_index, uint32_t not_immediate, uint32_t size) {
    auto& arg = blocks[cur_block].args[arg_index];

    if (not_immediate == 1) {
        arg.type = static_cast<PacArgType>(*stack_top->PAC_PC);
    } else {
        arg.type = None;
    }

    if (size == 4) {
        push_pc(4);
        std::memcpy(&arg.value, stack_top->PAC_PC, 4);
    }
    else if (size == 2) {
        push_pc(2);

        uint32_t storage;
        std::memcpy(&storage, stack_top->PAC_PC, 2);

        arg.value.as_int = storage;
    }
    else if (size == 1) {
        arg.value.as_int = *stack_top->PAC_PC;
    }

    return get_arg_ptr(arg_index);
}

void BasePacContext::debug_logger(uint32_t arg_index) {
    if ((logging_mask & 2) == 0) {
        return;
    }
    auto arg = blocks[cur_block].args[arg_index];

    // TODO: log
    // [
    // Arg.%02d : arg_index
    switch (arg.type) {
        case FloatGlobal:
        case FloatLocal:
        case FloatImm:
        case IntGlobal:
        case IntLocal:
        case IntImm:
        case Index:
        case None:
            break;
    }
    auto* arg_ptr = get_arg_ptr(arg_index);
    if (arg.type | FloatGlobal | FloatLocal | FloatImm) {
        // %f
    } else {
        // %d
    }
    // ]
}

void BasePacContext::get_string_argument(char *out) {
    auto len = std::strlen(stack_top->PAC_PC);
    std::memcpy(out, stack_top->PAC_PC, len + 1);
    stack_top->PAC_PC += len + 1;
}

void BasePacContext::make_new_block() {
    auto PAC_start = stack_top->PAC_start;
    ++cur_block;
    std::memset(&blocks[cur_block], 0x0, sizeof(blocks[cur_block]));
    auto undone = blocks[cur_block].undone_calls_cnt;
    stack_top = blocks[cur_block].stack + undone;
    blocks[cur_block].stack[undone].PAC_start = PAC_start;
}

void BasePacContext::revert_to_previous_block() {
    --cur_block;
    auto undone = blocks[cur_block].undone_calls_cnt;
    stack_top = blocks[cur_block].stack + undone;
}

uint32_t BasePacContext::get_blocks_count() {
    return cur_block;
}

uint32_t BasePacContext::set_blocks_count(uint32_t value) {
    uint32_t old = cur_block;
    cur_block = value;
    return old;
}

void BasePacContext::push_pc(uint32_t alignment) {
    // Hack
    auto ptr_as_int = reinterpret_cast<std::uintptr_t>(stack_top->PAC_PC);

    auto mask = ~(alignment - 1);
    ptr_as_int = mask & (ptr_as_int + alignment) - 1;

    stack_top->PAC_PC = reinterpret_cast<char*>(ptr_as_int);
}