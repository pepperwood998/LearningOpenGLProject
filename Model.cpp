#include "Model.hpp"
#include <glm/gtc/type_ptr.hpp>
#define  STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Log.hpp"

// Helper function declaration
GLuint GenTextureFromFile (const char *tex_file_name, const std::string &directory);

void Model::Draw (Shader shader)
{
    for (Mesh mesh : _meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel (const std::string &model_path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        RoadGL::Log("ERROR::ASSIMP::MODEL_LOADING", importer.GetErrorString());
        return;
    }

    _directory = model_path.substr(0, model_path.find_last_of('/'));
    this->ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode (const aiNode *node, const aiScene *scene)
{
    // process 
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(this->ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        this->ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh (const aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // fill vertices data
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;

        glm::vec3 vec;
        // get vertex position
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex._position = vec;
        // get normal vector
        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex._normal = vec;
        // get texture coordinate
        glm::vec2 tex_coord;
        if (mesh->mTextureCoords[0])
        {
            tex_coord.x = mesh->mTextureCoords[0][i].x;
            tex_coord.y = mesh->mTextureCoords[0][i].y;
        } else
        {
            tex_coord = glm::vec2(0.0f);
        }
        vertex._tex_coord = tex_coord;

        vertices.push_back(vertex);
    }

    // fill indices data
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // fill textures data
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // get diffuse maps
        std::vector<Texture> diffuse_maps = this->LoadTextureMaterials (material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        // get specular maps
        std::vector<Texture> specular_maps = this->LoadTextureMaterials (material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadTextureMaterials (const aiMaterial *material, aiTextureType texture_type, const char *tex_type_name)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(texture_type); ++i)
    {
        aiString tex_file_name;
        material->GetTexture(texture_type, i, &tex_file_name);

        bool skip = false;
        Texture texture;
        // check if current texture has been loaded
        for (unsigned int j = 0; j < _loaded_textures.size(); ++j)
        {
            texture = _loaded_textures[i];
            if (std::strcmp(tex_file_name.C_Str(), texture._file_name.c_str()) == 0) 
            {
                skip = true;
                break;
            }
        }
        // skip this texture if it has been loaded
        if (!skip)
        {
            texture._id        = GenTextureFromFile(tex_file_name.C_Str(), _directory);
            texture._type_name = tex_type_name;
            texture._file_name = tex_file_name.C_Str();

            _loaded_textures.push_back(texture);
        }
        
        textures.push_back(texture);
    }

    return textures;
}

// Helper function
GLuint GenTextureFromFile (const char *tex_file_name, const std::string &directory)
{
    GLuint texture;
    glGenTextures(1, &texture);

    std::string tex_path = directory + '/' + std::string(tex_file_name);
    int width, height, num_channels;
    unsigned char *data;

    data = stbi_load(tex_path.c_str(), &width, &height, &num_channels, 0);
    if (data)
    {
        glBindTexture  (GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum format;
        switch (num_channels)
        {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
        
            default:
                format = GL_RED;
                break;
        }

        glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        RoadGL::Log("ERROR::TEXTURE::INIT", "Texture loading failed");
    }
    stbi_image_free(data);

    return texture;
}
