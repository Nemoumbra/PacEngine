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
    REGISTER_COMMAND(cmd_sub);
    REGISTER_COMMAND(cmd_mul);
    REGISTER_COMMAND(cmd_div);
    REGISTER_COMMAND(cmd_inc);
    REGISTER_COMMAND(cmd_dec);
    REGISTER_COMMAND(cmd_loop);
    REGISTER_COMMAND(cmd_waitFrame);
    REGISTER_COMMAND(cmd_waitTime);
    REGISTER_COMMAND(cmd_rand);
    REGISTER_COMMAND(cmd_iand);
    REGISTER_COMMAND(cmd_ior);
    REGISTER_COMMAND(cmd_ixor);
    REGISTER_COMMAND(cmd_irol);
    REGISTER_COMMAND(cmd_iror);
    REGISTER_COMMAND(cmd_ifEQ);
    REGISTER_COMMAND(cmd_ifNE);
    REGISTER_COMMAND(cmd_ifLSE);
    REGISTER_COMMAND(cmd_ifLBE);
    REGISTER_COMMAND(cmd_ifLS);
    REGISTER_COMMAND(cmd_ifLB);
    REGISTER_COMMAND(cmd_ifAND);
    REGISTER_COMMAND(cmd_ifOR);
    REGISTER_COMMAND(cmd_ifCallEQ);
    REGISTER_COMMAND(cmd_ifCallNE);
    REGISTER_COMMAND(cmd_ifCallLSE);
    REGISTER_COMMAND(cmd_ifCallLBE);
    REGISTER_COMMAND(cmd_ifCallLS);
    REGISTER_COMMAND(cmd_ifCallLB);
    REGISTER_COMMAND(cmd_ifCallAND);
    REGISTER_COMMAND(cmd_ifCallOR);
    REGISTER_COMMAND(cmd_flgSet);
    REGISTER_COMMAND(cmd_flgClr);
    REGISTER_COMMAND(cmd_flgSetId);
    REGISTER_COMMAND(cmd_flgClrId);
    REGISTER_COMMAND(cmd_flgMov);
    REGISTER_COMMAND(cmd_flgAND);
    REGISTER_COMMAND(cmd_flgOR);
    REGISTER_COMMAND(cmd_flgZERO);
    REGISTER_COMMAND(cmd_inxJmp);
    REGISTER_COMMAND(cmd_stkDec);
    REGISTER_COMMAND(cmd_stkClr);
    REGISTER_COMMAND(cmd_break);
    REGISTER_COMMAND(cmd_string);
    REGISTER_COMMAND(cmd_print);
    REGISTER_COMMAND(cmd_textOut);
    REGISTER_COMMAND(cmd_setSleep);
    REGISTER_COMMAND(cmd_sinf);
    REGISTER_COMMAND(cmd_cosf);
    REGISTER_COMMAND(cmd_atan2f);
    REGISTER_COMMAND(cmd_abs);
    REGISTER_COMMAND(cmd_sqrt);
    REGISTER_COMMAND(cmd_setLabelId);
    REGISTER_COMMAND(cmd_callLabelId);
    REGISTER_COMMAND(cmd_jmpLabelId);
    REGISTER_COMMAND(cmd_callLabel);
    REGISTER_COMMAND(cmd_jmpLabel);
    REGISTER_COMMAND(cmd_mod);
    REGISTER_COMMAND(cmd_memset);
    REGISTER_COMMAND(cmd_memcpy);
    REGISTER_COMMAND(cmd_flgMemset);
    REGISTER_COMMAND(cmd_flgMemcpy);
    REGISTER_COMMAND(cmd_stringEnable);
    REGISTER_COMMAND(cmd_srand);
    REGISTER_COMMAND(cmd_initArray);
    REGISTER_COMMAND(cmd_addArray);
    REGISTER_COMMAND(cmd_setArray);
    REGISTER_COMMAND(cmd_getArray);
    REGISTER_COMMAND(cmd_F32toF16);
    REGISTER_COMMAND(cmd_F16toF32);
    REGISTER_COMMAND(cmd_getElapsedTime);

};