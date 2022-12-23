#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "CubeTexture.h"

class Skybox
{
public:
    Skybox();
    ~Skybox();

    void Init();
    void SetupCubeMap();

    inline VertexArray *GetVAO() const { return m_VAO; }
    inline Shader *GetShader() const { return m_Shader; }
    inline CubeTexture *GetTexture() const { return m_Texture; }

    void Draw();

private:
    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    CubeTexture *m_Texture;
    Shader *m_Shader;
};


#endif // SKYBOX_H