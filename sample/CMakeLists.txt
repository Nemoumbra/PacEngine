cmake_minimum_required(VERSION 3.26)

project(PacEngine_Sample)

add_executable(
        Interpreter
        main.cpp
)

target_link_libraries(Interpreter PacEngine)
