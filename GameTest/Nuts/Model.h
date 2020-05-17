#pragma once

#include "TypeAliases.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Logger.h"

namespace Nuts {
    class Model
    {
    private:
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;
    public:
        Model(std::string path)
        {
            LoadModel(path);
        }
        void Draw(Shader shader) {
            for (Mesh& mesh : meshes) {
                mesh.Draw(shader);
            }
        }
    private:
        void LoadModel(std::string path) {
            Assimp::Importer import;
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                Logger::Error("Model::LoadModel", import.GetErrorString());
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));
            ProcessNode(scene->mRootNode, scene);
        }

        void ProcessNode(aiNode* node, const aiScene* scene) {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(ProcessMesh(mesh, scene));
            }
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene);
            }
        }

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                auto& v = mesh->mVertices[i];
                auto& vn = mesh->mNormals[i];

                Vertex vertex;
                vertex.position = glm::vec3(v.x, v.y, v.z);
                vertex.normal = glm::vec3(vn.x, vn.y, vn.z);

                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    auto& coords = mesh->mTextureCoords[0][i];
                    vertex.textureCoords = glm::vec2(coords.x, coords.y);
                }
                else {
                    vertex.textureCoords = glm::vec2(0.0f, 0.0f);
                }

                auto& tan = mesh->mTangents[i];
                auto& bitan = mesh->mBitangents[i];
                vertex.tangent = glm::vec3(tan.x, tan.y, tan.z);
                vertex.bitangent = glm::vec3(bitan.x, bitan.y, bitan.z); ;
                vertices.push_back(vertex);
            }
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            return Mesh(vertices, indices, textures);
        }

        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
        {
            std::vector<Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString aiStr;
                mat->GetTexture(type, i, &aiStr);
                std::string str(aiStr.C_Str());
                auto it = std::find(textures_loaded.begin(), textures_loaded.end(), str);

                if (it != textures_loaded.end()) {
                    textures.push_back(*it);
                }
                else {
                    Texture texture;
                    texture.id = TextureFromFile(str, this->directory, false);
                    texture.type = typeName;
                    texture.path = str;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }
            return textures;
        }

        unsigned int TextureFromFile(const std::string& path, const std::string& directory, bool gamma)
        {
            std::string filename = directory + '/' + path;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1) {
                    format = GL_RED;
                }
                else if (nrComponents == 3) {
                    format = GL_RGB;
                }
                else if (nrComponents == 4) {
                    format = GL_RGBA;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else
            {
                Logger::Error("Model", "Texture failed to load at path: " + path);
            }
            stbi_image_free(data);
            return textureID;
        }
    };
}