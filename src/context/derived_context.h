//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "base_context.h"

class DerivedPacContext: public BasePacContext {
protected:
    RegisterGroup* locals_ptr;
public:
    DerivedPacContext(uint32_t int_reg_cnt, uint32_t float_reg_cnt);
    ~DerivedPacContext() override;
    RegisterGroup* get_locals() override;
};