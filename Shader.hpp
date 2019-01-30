#ifndef __SHADER_HPP__
#define __SHADER_HPP__
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
    GLuint _ID;

    static GLuint CompileShader (GLenum shader_type, const char *shader_src);
    static GLuint LinkShaders (GLuint vertex_shader, GLuint fragment_shader);
    
public:
    Shader (const char *vertex_path, const char *fragment_path);
    void Use ();
    void SetMat4 (const char *name, glm::mat4 value);
};

#endif // __SHADER_HPP
