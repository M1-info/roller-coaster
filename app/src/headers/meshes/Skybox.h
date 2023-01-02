#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Mesh.h"
#include "utils/CubeTexture.h"

class Skybox : public Mesh
{
public:
    Skybox();

    void Init();
    void SetupCubeMap();

    inline CubeTexture *GetTexture() const { return m_Texture; }

    void Draw() override;
    void Update() override;

private:
    CubeTexture *m_Texture;
};

#endif // SKYBOX_H