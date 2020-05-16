#pragma once

#include "TypeAliases.h"
#include "BasicTypes.h"

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
            std::vector<uint> indices
            //std::vector<Texture> textures
        ) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = {}; // textures;
            InitializeBuffers();
        }

        void Draw()
        {
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
            glBindVertexArray(0);
        }
    };
}