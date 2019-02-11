#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Shader.hpp"

class Model
{
private:
    std::vector<Mesh> _meshes;
    std::string       _directory;

    void LoadModel                            (const std::string &model_path);
    void ProcessNode                          (const aiNode *node, const aiScene *scene);
    Mesh ProcessMesh                          (const aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> LoadTextureMaterials (const aiMaterial *material, aiTextureType texture_type, const char *tex_type_name);

public:
    Model (const char *model_path) { this->LoadModel(model_path); }

    void Draw (Shader shader);
};

#endif // __MODEL_HPP__
