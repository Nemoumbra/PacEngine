//
// Created by Nemoumbra on 06.01.2025.
//

#include "derived_context.h"

DerivedPacContext::DerivedPacContext(uint32_t int_reg_cnt, uint32_t float_reg_cnt):
    BasePacContext(int_reg_cnt, float_reg_cnt),
    locals_ptr(&locals)
    {}

DerivedPacContext::~DerivedPacContext() {
    // probably nothing important for us?
}

RegisterGroup *DerivedPacContext::get_locals() {
    return locals_ptr;
}