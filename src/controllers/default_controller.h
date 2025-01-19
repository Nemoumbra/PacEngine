//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "base/default_controller_base.h"


struct DefaultController : DefaultControllerBase {
    DefaultController();
    ~DefaultController() override = default;
    void get_handler(Handler *out, uint32_t index) override;
    virtual void cmd_end_logger();

    REGISTER_COMMAND(cmd_end);
    REGISTER_COMMAND(cmd_jmp);
    REGISTER_COMMAND(cmd_call);
    REGISTER_COMMAND(cmd_result);
    REGISTER_COMMAND(cmd_resJmp);
    REGISTER_COMMAND(cmd_resCall);
    REGISTER_COMMAND(cmd_mov);
    REGISTER_COMMAND(cmd_add);
};