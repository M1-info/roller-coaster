#pragma once
#ifndef GROUND_H
#define GROUND_H

#include <glm/glm.hpp>
#include "Mesh.h"
#include "Object3D.h"

class Ground : public Mesh
{
public:
    Ground();
    static std::shared_ptr<Ground> Create();

    void Draw() override;
    void Update() override;
};

#endif // GROUND_H