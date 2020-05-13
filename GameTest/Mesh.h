#pragma once

#include "TypeAliases.h"
#include "BasicTypes.h"

class Mesh {
private:
    uint m_Vao;
    uint m_Vbo;
    uint m_Ebo;
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
    }
};