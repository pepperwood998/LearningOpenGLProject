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
)
target_link_libraries(
    Run PRIVATE
    glfw
)
