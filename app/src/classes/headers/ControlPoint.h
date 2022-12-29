#pragma once
#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "Mesh.h"

class ControlPoint : public Mesh
{
public:
    ControlPoint(glm::vec3 point, int index);

    void Draw() override;
    void Update() override{};

    glm::vec3 *GetPoint() { return &m_Vertices[0]; }
};

#endif // CONTROLPOINT_H