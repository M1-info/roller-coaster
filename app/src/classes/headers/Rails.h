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

class Rails
{
public: 
    Rails();
    Rails(std::vector<glm::vec3> controlPoints);
    ~Rails();

    void CreateFromControlPoints(std::vector<glm::vec3> controlPoints);

    inline std::vector<Vertex> GetVertices() { return m_Vertices; }
    inline Shader* GetShader() { return m_Shader; }

    void Draw();

private: 
    std::string m_Name = "Rails";
    VertexArray *m_VAO_lines;
    VertexArray *m_VAO_points;
    VertexBuffer *m_VBO_lines;
    VertexBuffer *m_VBO_points;
    Shader *m_Shader;
    
    std::vector<Vertex> m_Vertices;
    std::vector<glm::vec3> m_ControlPoints;
};

#endif // RAILS_H