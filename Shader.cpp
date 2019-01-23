#include "Shader.hpp"
#include "Log.hpp"

#include <fstream>
#include <sstream>
#include <string>

Shader::Shader (const char *vertex_path, const char *fragment_path)
{
    std::ifstream vert_file;
    std::ifstream frag_file;
    vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    std::string vert_src;
    std::string frag_src;
    try
    {
        std::ostringstream vert_stream;
        std::ostringstream frag_stream;

        vert_file.open(vertex_path);
        frag_file.open(fragment_path);

        vert_stream << vert_file.rdbuf();
        frag_stream << frag_file.rdbuf();
        vert_file.close();
        frag_file.close();

        vert_src = vert_stream.str();
        frag_src = frag_stream.str();

    } catch (std::fstream::failure ex)
    {
        unnamed::Log("ERROR::SHADER::FILE", "Shader file read failed");
    }

    GLuint vert_shader = CompileShader(GL_VERTEX_SHADER, vert_src.c_str());
    GLuint frag_shader = CompileShader(GL_FRAGMENT_SHADER, frag_src.c_str());
    _ID = LinkShaders(vert_shader, frag_shader);
}

void Shader::Use ()
{
    glUseProgram(_ID);
}

void Shader::SetMat4 (const char *name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(_ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

GLuint Shader::CompileShader (GLenum shader_type, const char *shader_src)
{
    GLuint shader;
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);

    // check for compilation errors
    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        unnamed::Log("ERROR::SHADER::COMPILATION", info_log);
    }

    return shader;
}

GLuint Shader::LinkShaders (GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint program;
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        unnamed::Log("ERROR::SHADER::LINKING", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}
