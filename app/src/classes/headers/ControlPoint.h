#pragma once
#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include "Mesh.h"

class ControlPoint : public Mesh
{
public:
    ControlPoint(glm::vec3 point);
    inline glm::vec3 GetVertices() { return glm::vec3(m_Vertices[0].x, m_Vertices[0].y, m_Vertices[0].z); }

    void Draw() override;

};

#endif // CONTROLPOINT_H