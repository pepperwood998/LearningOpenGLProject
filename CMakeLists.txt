cmake_minimum_required(VERSION 3.12)
project(
    LearningProject 
    VERSION 1.0.0
    DESCRIPTION "OpenGL Learning with Git"
)

find_package(glfw3 3.2 REQUIRED)

add_executable(Run)
target_sources(
    Run PRIVATE
    Main.cpp
    Log.cpp
    Log.hpp
    Shader.hpp
    Shader.cpp
    glad.c
)
target_include_directories(
    Run PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
target_link_libraries(
    Run PRIVATE
    glfw
)

if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(
        TARGET Run POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res" "$<TARGET_FILE_DIR:Run>/res"
    )
endif()

