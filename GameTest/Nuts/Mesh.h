#pragma once

#include "TypeAliases.h"
#include "BasicTypes.h"
#include "Shader.h"

namespace Nuts {
    class Mesh {
    private:
        uint m_Vao;
        uint m_Vbo;
        uint m_Ebo;

    private:

        void InitializeBuffers() {
            glGenVertexArrays(1, &m_Vao);
            glGenBuffers(1, &m_Vbo);
            glGenBuffers(1, &m_Ebo);

            glBindVertexArray(m_Vao);

            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), &indices[0], GL_STATIC_DRAW);

            // Describe structure
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

            glBindVertexArray(0);
        }

    public:
        std::vector<Vertex> vertices;
        std::vector<uint> indices;
        std::vector<Texture> textures;

        Mesh(
            std::vector<Vertex> vertices,
            std::vector<uint> indices,
            std::vector<Texture> textures
        ) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            InitializeBuffers();
        }

        void Draw(Shader& shader)
        {
            uint diffuseNr = 1;
            uint specularNr = 1;

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                std::string number;
                std::string name = textures[i].type;

                if (name == "texture_diffuse") {
                    number = std::to_string(diffuseNr++);
                }
                else if (name == "texture_specular")
                {
                    number = std::to_string(specularNr++);
                }
                std::string parameterName = "material." + name + number;

                shader.SetFloat(parameterName, i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
            glBindVertexArray(0);
        }
    };
}