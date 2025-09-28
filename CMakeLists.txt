cmake_minimum_required(VERSION 3.28)
project(prog_2025)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE ALL_CPP_FILES "code/*.cpp")

add_compile_options(-Wall -Wextra -Wpedantic)

foreach(cpp_file ${ALL_CPP_FILES})
    get_filename_component(target_name ${cpp_file} NAME_WE)
    add_executable(${target_name} ${cpp_file})

    set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()
