#pragma once
#ifndef RAILS_H
#define RAILS_H

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

class Rails : public Mesh
{
public:
    Rails();
    static std::shared_ptr<Rails> Create(std::vector<glm::vec3> controlPoints);
    void RemoveChildren(std::shared_ptr<Mesh> child) override;
    void Draw() override;
    void Update() override;
};

#endif // RAILS_H