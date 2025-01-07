//
// Created by Nemoumbra on 06.01.2025.
//

#pragma once

#include "base_controller.h"
#include "../../instruction.h"

struct CustomControllerBase : BaseController {
    CustomControllerBase() = default;

    ~CustomControllerBase() override = default;
    int get_something() override;
    virtual void custom_action();
};
