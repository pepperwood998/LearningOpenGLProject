#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Log.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int WIDTH   = 800;
const int HEIGHT  = 600;
const char *title = "Learning OpenGL Project";

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 2.5f);
float mouse_last_x = WIDTH / 2;
float mouse_last_y = HEIGHT / 2;
float is_focused   = false;
float delta_time      = 0.0f;
float last_frame_time = 0.0f;

// process inputs coming to "window"
void ProcessInput (GLFWwindow *window);

// Mouse movement callback
void cb_MouseMove(GLFWwindow *window, double pos_x, double pos_y);

// Event Callback
void cb_FramebufferSize (GLFWwindow *window, int width, int height);

int main (int argc, char const *argv[])
{
    // GLFW
    // --------------------
    glfwInit();
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cb_MouseMove);

    // GLAD
    // --------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        RoadGL::Log("ERROR::GLAD::INIT", "OpenGL function pointers load failed");
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, cb_FramebufferSize);
    glEnable(GL_DEPTH_TEST);

    Shader shader("res/shader/shader.vs", "res/shader/shader.fs");
    Shader shader_light("res/shader/shader_light.vs", "res/shader/shader_light.fs");

    // Vertices data
    // --------------------
    float vertices[] = 
    {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f,
    };

    // The object
    GLuint VBO;
    GLuint VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Light object
    GLuint VAO_light;
    glGenVertexArrays(1, &VAO_light);

    glBindVertexArray(VAO_light);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // One-time-set variables
    // --------------------
    glm::mat4 model      = glm::mat4(1.0f);
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    glm::vec3 object_col = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 light_col  = glm::vec3(1.0f);

    glm::mat4 light_model = glm::mat4(1.0f);
    light_model = glm::translate(light_model, glm::vec3(0.5f, 0.0f, 1.5f));
    light_model = glm::scale(light_model, glm::vec3(0.2f));

    shader.Use();
    shader.SetMat4("model", model);
    shader.SetMat4("projection", projection);
    shader.SetVec3("object_col", object_col);
    shader.SetVec3("light_col", light_col);

    shader_light.Use();
    shader_light.SetMat4("model", light_model);
    shader_light.SetMat4("projection", projection);

    // Game loop
    // --------------------
    while (!glfwWindowShouldClose(window))
    {
        float current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        ProcessInput(window);

        glClearColor(0.0f, 0.28f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(VAO);

        view = camera.GetViewMatrix();

        shader.Use();
        shader.SetMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(VAO_light);
        shader_light.Use();
        shader_light.SetMat4("view", view);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    // --------------------
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
