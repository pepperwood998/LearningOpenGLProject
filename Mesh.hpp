#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

#include "Shader.hpp"

struct Vertex
{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec3 _tex_coord;
};

struct Texture
{
    GLuint      _id;
    std::string _type_name;
};

class Mesh
{
private:
    std::vector<Vertex>       _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture>      _textures;

    GLuint _VAO;
    GLuint _VBO;
    GLuint _EBO;

    void InitMesh ();

public:
    Mesh (const std::vector<Vertex>       &vertices,
          const std::vector<unsigned int> &indices,
          const std::vector<Texture>      &textures);

    void Draw (Shader shader);
};

#endif // __MESH_HPP__
