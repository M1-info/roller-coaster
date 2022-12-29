#pragma once
#ifndef RAILS_H
#define RAILS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BezierCurve.h"
#include "Mesh.h"
#include "Shader.h"
#include "ControlPoint.h"
#include "OBJLoader.h"
#include "Rail.h"

class Rails : public Mesh
{
public:
    Rails();
    static std::shared_ptr<Rails> Create(std::vector<glm::vec3> controlPoints);
    void RemoveChildren(std::shared_ptr<Mesh> child) override;
    void Draw() override;
    void Update() override;
    void UpdateRails();

    inline std::vector<std::shared_ptr<Rail>> GetRails() { return m_Rails; }

private: 
    std::vector<std::shared_ptr<Rail>> m_Rails;

public:
    bool m_DrawRails = false;
};

#endif // RAILS_H