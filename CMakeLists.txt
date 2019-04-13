cmake_minimum_required(VERSION 3.12)
project(
    LearningProject 
    VERSION 1.0.0
    DESCRIPTION "OpenGL Learning with Git"
)

find_package(glfw3 3.2 REQUIRED)
find_package(ASSIMP REQUIRED)

add_executable(Run)

target_sources(
    Run PRIVATE
    Main.cpp
    Log.hpp    Log.cpp
    Shader.hpp Shader.cpp
    Camera.hpp Camera.cpp
    Mesh.hpp   Mesh.cpp
    Model.hpp  Model.cpp
    glad.c
)
target_include_directories(
    Run PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/lib"
)
target_link_libraries(
    Run PRIVATE
    glfw
    ${ASSIMP_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

install(
    TARGETS Run
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/"
)
install(
    DIRECTORY res/
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/res"
)

