//
// Created by Nemoumbra on 06.01.2025.
//

#include "script_controller.h"
#include "../../context/base_context.h"

// TODO: macro this
static Instruction instructions[] = {
        {0x0, nullptr, "null"},
        {0x1, static_cast<Handler>(&ScriptController::dummy), "dummy"},
        {0x2, static_cast<Handler>(&ScriptController::setScriptLabelTable), "setScriptLabelTable"},
        {0x3, static_cast<Handler>(&ScriptController::setCommandMonitor), "setCommandMonitor"},
        {0x4, static_cast<Handler>(&ScriptController::getPack), "getPack"},
};

constexpr auto instructions_cnt = sizeof(instructions) / sizeof(instructions[0]);

ScriptController::ScriptController():
        ScriptControllerBase(0x10, ::instructions, instructions_cnt) {}


/// Instructions
void ScriptController::dummy(BasePacContext *ctx, int sec_id, int instr_id) {

}

void ScriptController::setScriptLabelTable(BasePacContext *ctx, int sec_id, int instr_id) {

}

void ScriptController::setCommandMonitor(BasePacContext *ctx, int sec_id, int instr_id) {

}

void ScriptController::getPack(BasePacContext *ctx, int sec_id, int instr_id) {

}