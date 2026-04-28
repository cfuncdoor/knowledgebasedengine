cmake_minimum_required(VERSION 3.10)
project(knowledgebasedengine LANGUAGES C)

add_executable(knowledge 
    src/main.c
    src/core/math/vector/vec2.c
    src/core/renderer/renderer.c
    src/core/types/string_view.c
    src/core/logger/logger.c
    src/core/window/window.c
    src/core/application.c
    src/core/types/color.c
    include/core/renderer/dx9.h
    include/core/window/win32.h
    src/core/renderer/dx9.c)

target_link_libraries(knowledge PRIVATE d3d9)

set_target_properties(knowledge PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

target_include_directories(knowledge PRIVATE "include")
target_compile_options(knowledge PRIVATE -W3)