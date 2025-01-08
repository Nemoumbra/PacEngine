//
// Created by Nemoumbra on 08.01.2025.
//

#include "debugger.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>

void Debugger::OnUnknownSection(uint32_t sec_id) {
    PrintLog("Unknown section: 0x{:x}\n", sec_id);
}

void Debugger::OnUnknownInstruction(uint32_t sec_id, uint32_t cmd_id) {
    PrintLog("Unknown instruction: 0x25 {:x} {:x}\n", sec_id, cmd_id);
}

void Debugger::OnCmdEnd() {
    PrintLog("cmd_end: ending script execution!\n");
}

