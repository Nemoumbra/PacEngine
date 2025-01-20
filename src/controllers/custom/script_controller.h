//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "../custom/script_controller_base.h"
#include "../../context/pac_context.h"

struct ScriptController : ScriptControllerBase {
    PacContext* context;

    explicit ScriptController(PacContext* ctx);

    ~ScriptController() override = default;

    REGISTER_COMMAND(dummy);
    REGISTER_COMMAND(setScriptLabelTable);
    REGISTER_COMMAND(callScriptLabel);
    REGISTER_COMMAND(callScriptLabelId);
    REGISTER_COMMAND(setCommandMonitor);
    REGISTER_COMMAND(getPack);
};