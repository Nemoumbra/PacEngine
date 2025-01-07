cmake_minimum_required(VERSION 3.26)

project(PacEngine_Library)

add_library(
        PacEngine
        STATIC
        context/base_context.cpp
        context/derived_context.cpp
        context/pac_context.cpp
        controllers/base/default_controller_base.cpp
        controllers/default_controller.cpp
        runtime/pac_flags.cpp
        runtime/pac_labels.cpp
        runtime/pac_stack.cpp
        runtime/register_group.cpp
        runtime/pac_runtime.cpp
)