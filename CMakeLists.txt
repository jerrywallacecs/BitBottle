cmake_minimum_required(VERSION 3.20)

project(BitBottle
    VERSION 1.0
    LANGUAGES C
)

add_library(BitBottle STATIC
    source/ErrorHandling.c
    source/BitBottle.c
)

target_include_directories(BitBottle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(BitBottle PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
)