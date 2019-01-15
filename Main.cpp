#include <GLFW/glfw3.h>
#include "Log.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *title = "Learning OpenGL Project";

// process inputs coming to "window"
void ProcessInput(GLFWwindow *window);

int main(int argc, char const *argv[])
{
    // GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        unnamed::Log("ERROR::GLFW::INIT", "Window creation failed");
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Game Loop
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
