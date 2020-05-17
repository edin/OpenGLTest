#pragma once

#include "TypeAliases.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Nuts {
    class Model
    {
    private:
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
                //TODO: Log error
                //cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
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
        }
    };
}