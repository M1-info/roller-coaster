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
#include "OBJLoader.h"

class Rails : public Mesh
{
public: 
    Rails(std::vector<glm::vec3> controlPoints);
    void Draw() override;

private: 
    VertexArray *m_VAO_lines;
    VertexArray *m_VAO_points;
    VertexBuffer *m_VBO_lines;
    VertexBuffer *m_VBO_points;
    
    std::vector<glm::vec3> m_ControlPoints;
};

#endif // RAILS_H