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
    REGISTER_COMMAND(labelToInt);
    REGISTER_COMMAND(createScript);
    REGISTER_COMMAND(setGroupId);
    REGISTER_COMMAND(getGroupId);
    REGISTER_COMMAND(getParentGroupId);
    REGISTER_COMMAND(killChildren);
    REGISTER_COMMAND(killScript);
    REGISTER_COMMAND(setAddress);
    REGISTER_COMMAND(setLocalValue);
    REGISTER_COMMAND(getLocalValue);

};