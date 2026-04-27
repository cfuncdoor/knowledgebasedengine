cmake_minimum_required(VERSION 3.10)
project(knowledgebasedengine LANGUAGES C)

add_executable(knowledge 
    src/main.c
    src/core/math/vector/vec2.c
    src/core/renderer/renderer.c
    src/core/types/string_view/string_view.c
    src/core/logger/logger.h
    src/core/logger/logger.c
    src/core/window/window.h
    src/core/window/window.c
    src/core/application.h
    src/core/application.c
    src/core/math/vertex/vertex.h)

target_link_libraries(knowledge PRIVATE d3d9)

set_target_properties(knowledge PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)