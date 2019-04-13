#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <algorithm>
#include <string>

#include "Log.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.hpp"

using namespace RoadGL;

const int WIDTH   = 800;
const int HEIGHT  = 600;
const char *title = "Learning OpenGL Project";

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 2.5f);
float mouse_last_x = WIDTH / 2;
float mouse_last_y = HEIGHT / 2;
float is_focused   = false;

float delta_time      = 0.0f;
float last_frame_time = 0.0f;

/**
 *  process inputs coming to "window"
 */
void ProcessInput (GLFWwindow *window);

// Callback Functions
// --------------------
/**
 *  Mouse movement callback
 */
void cb_MouseMove (GLFWwindow *window, double pos_x, double pos_y);
/**
 *  Window resize callback
 */
void cb_FramebufferSize (GLFWwindow *window, int width, int height);

int main (int argc, char const *argv[])
{
    // GLFW
    // --------------------
    glfwInit      ();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        RoadGL::Log("ERROR::GLFW::INIT", "Window creation failed");
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode        (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cb_MouseMove);

    // GLAD
    // --------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        RoadGL::Log("ERROR::GLAD::INIT", "OpenGL function pointers load failed");
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, cb_FramebufferSize);

    // One-time-set variables
    // --------------------
    Shader shader  = Shader("res/shader/shader.vs", "res/shader/shader.fs");
    Model nanosuit = Model ("res/model/nanosuit/nanosuit.obj");

    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model      = glm::scale      (model, glm::vec3(0.2f));
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    glm::mat4 normal_mat = glm::mat4(1.0f);

    normal_mat = glm::transpose(glm::inverse(model));

    // Object-shader's uniforms setting
    shader.Use    ();
    shader.SetMat4("model",      model);
    shader.SetMat4("projection", projection);

    shader.SetVec3("dir_light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    shader.SetVec3("dir_light.ambient",   glm::vec3( 0.5f,  0.5f,  0.5f));
    shader.SetVec3("dir_light.diffuse",   glm::vec3( 0.5f,  0.5f,  0.5f));
    shader.SetVec3("dir_light.specular",  glm::vec3( 1.0f,  1.0f,  1.0f));

    // Game loop
    // --------------------
    while (!glfwWindowShouldClose(window))
    {
        // calculate frame's delta time
        float current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        ProcessInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.GetViewMatrix();

        shader.Use    ();
        shader.SetMat4("view", view);
        shader.SetVec3("view_pos", camera.GetPos());
        
        nanosuit.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents ();
    }

    // Clean up
    // --------------------
    glfwTerminate();
    return 0;
}

void ProcessInput (GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    } else
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            camera.Move(CameraDirection::FORWARD, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.Move(CameraDirection::BACKWARD, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.Move(CameraDirection::LEFT, delta_time);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.Move(CameraDirection::RIGHT, delta_time);
        }
    }
}

void cb_MouseMove (GLFWwindow *window, double pos_x, double pos_y)
{
    if (!is_focused)
    {
        is_focused = true;
        mouse_last_x = pos_x;
        mouse_last_y = pos_y;
    }

    float offset_x = pos_x - mouse_last_x;
    float offset_y = mouse_last_y - pos_y;

    mouse_last_x = pos_x;
    mouse_last_y = pos_y;

    camera.Look(offset_x, offset_y);
}

void cb_FramebufferSize (GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
