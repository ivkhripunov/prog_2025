cmake_minimum_required(VERSION 3.28)
project(prog_2025)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(third_party/googletest)

include(CTest)
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/code)

set(Boost_USE_STATIC_LIBS    OFF)
set(Boost_USE_STATIC_RUNTIME OFF)

#############################################################################################

find_package(Boost 1.88 REQUIRED COMPONENTS
        filesystem
        locale
        log
        python
        system
        timer
        unit_test_framework
)

#############################################################################################

include_directories(${Boost_INCLUDE_DIRS})

file(GLOB_RECURSE ALL_CPP_FILES "code/*.cpp")
list(REMOVE_ITEM ALL_CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/code/sem_2/task_12/06_05/06_05.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/code/sem_2/task_12/06_05/library_v1.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/code/sem_2/task_12/06_05/library_v2.cpp"
)

add_compile_options(-Wall -Wextra -Wpedantic)

foreach(cpp_file ${ALL_CPP_FILES})
    get_filename_component(target_name ${cpp_file} NAME_WE)
    get_filename_component(target_dir ${cpp_file} DIRECTORY)

    add_executable(${target_name} ${cpp_file})
    target_include_directories(${target_name} PRIVATE ${target_dir} ${Boost_INCLUDE_DIRS})
    target_link_libraries(${target_name} GTest::gtest GTest::gtest_main ${Boost_LIBRARIES})
    set_target_properties(${target_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()

add_library(shared_v1 SHARED code/sem_2/task_12/06_05/library_v1.cpp)
set_target_properties(shared_v1 PROPERTIES
        OUTPUT_NAME "shared_v1"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_include_directories(shared_v1 PRIVATE ${Boost_INCLUDE_DIRS})

add_library(shared_v2 SHARED code/sem_2/task_12/06_05/library_v2.cpp)
set_target_properties(shared_v2 PROPERTIES
        OUTPUT_NAME "shared_v2"
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
target_include_directories(shared_v2 PRIVATE ${Boost_INCLUDE_DIRS})

add_executable(06_05
        code/sem_2/task_12/06_05/06_05.cpp
)
target_include_directories(06_05 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/code/sem_2/task_12/06_05
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(06_05 ${Boost_LIBRARIES})
set_target_properties(06_05 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_dependencies(06_05 shared_v1 shared_v2)