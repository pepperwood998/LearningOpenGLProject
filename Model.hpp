#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Mesh.hpp"
#include "Shader.hpp"

class Model
{
private:
    std::vector<Mesh>     _meshes;
    std::string           _directory;
    std::vector<Texture> _loaded_textures;

    /**
     * start model-loading process
     */
    void LoadModel                            (const std::string &model_path);
    /**
     * iterate through all assimp's nodes, fill the meshes-data
     */
    void ProcessNode                          (const aiNode *node, const aiScene *scene);
    /**
     * process and get our Mesh from assimp's mesh
     */
    Mesh GetMesh                              (const aiMesh *mesh, const aiScene *scene);
    /**
     *  get textures depend on texture-type
     */
    std::vector<Texture> LoadTextureMaterials (const aiMaterial *material, aiTextureType texture_type, const char *tex_type_name);

public:
    Model (const char *model_path) { this->LoadModel(model_path); }

    void Draw (Shader shader);
};

#endif // __MODEL_HPP__
