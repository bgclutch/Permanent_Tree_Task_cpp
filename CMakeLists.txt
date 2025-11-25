cmake_minimum_required(VERSION 3.13)

project(persistent_tree)
enable_testing()
add_subdirectory(persistent_tree)
add_subdirectory(tests)

option(ENABLE_SAN OFF)

foreach(target persistent_tree tests)
    target_compile_features(${target} PRIVATE cxx_std_23)
endforeach()

target_compile_options(persistent_tree PRIVATE
    $<$<CONFIG:Debug>:
        -Wall
        -Wextra
        -Wpedantic
        -DDEBUG
        -Wmissing-declarations
        -Wcast-align
        -Wshadow
        -Wsign-conversion>
)

if (ENABLE_SAN)
    foreach(target persistent_tree tests)
        target_compile_options(${target} PRIVATE $<$<CONFIG:Debug>: -fsanitize=address -fsanitize=undefined>)
        target_link_options(${target} PRIVATE $<$<CONFIG:Debug>: -fsanitize=address -fsanitize=undefined>)
    endforeach()
endif()
