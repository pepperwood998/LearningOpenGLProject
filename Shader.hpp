#ifndef __SHADER_HPP__
#define __SHADER_HPP__
#include <glad/glad.h>

class Shader
{
private:
    GLuint _ID;
public:
    Shader(const char *vertex_path, const char *fragment_path);
    void Use();

    static GLuint CompileShader(GLenum shader_type, const char *shader_src);
    static GLuint LinkShaders(GLuint vertex_shader, GLuint fragment_shader);
};

#endif // __SHADER_HPP
