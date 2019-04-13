#include "Mesh.hpp"

RoadGL::Mesh::Mesh (const std::vector<Vertex>       &vertices,
            const std::vector<unsigned int> &indices,
            const std::vector<Texture>      &textures)
    : _vertices(vertices), _indices(indices), _textures(textures)
{
    this->InitMesh();
}

void RoadGL::Mesh::InitMesh ()
{
    glGenVertexArrays(1, &_VAO);
    glGenBuffers     (1, &_VBO);
    glGenBuffers     (1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer    (0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer    (1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer    (2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _tex_coord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void RoadGL::Mesh::Draw (Shader shader)
{
    unsigned int diffuse_num  = 1;
    unsigned int specular_num = 1;

    for (unsigned int i = 0; i < _textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture  (GL_TEXTURE_2D, _textures[i]._id);

        std::string tex_type = _textures[i]._type_name;
        std::string tex_type_num;

        if (tex_type == "texture_diffuse")
        {
            tex_type_num = std::to_string(diffuse_num++);
        } else if (tex_type == "texture_specular")
        {
            tex_type_num = std::to_string(specular_num++);
        }

        shader.SetInt(("material." + tex_type + tex_type_num).c_str(), i);
    }
    shader.SetFloat("material.shininess", 32.0f);
    
    glBindVertexArray(_VAO);
    glDrawElements   (GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
