#pragma once
#ifndef RAILS_H
#define RAILS_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <sys/stat.h>
#include <filesystem>
#include <cmath>

#include "buffers/VertexArray.h"
#include "buffers/VertexBuffer.h"

#include "curves/BezierCurve.h"
#include "curves/Bspline.h"

#include "meshes/Mesh.h"
#include "meshes/Rail.h"
#include "meshes/ControlPoint.h"

#include "utils/Shader.h"
#include "utils/OBJLoader.h"

#define RAIL_WIDTH 0.5f

class Rails : public Mesh
{
public:
    Rails();
    static std::shared_ptr<Rails> Create();
    void RemoveChildren(std::shared_ptr<Mesh> child) override;
    void Draw() override;
    void Update() override;
    void UpdateRails();

    void GenerateControlPoints(std::vector<glm::vec3> controlPoints);

    inline std::vector<std::shared_ptr<Rail>> GetRails() { return m_Rails; }

private:
    std::vector<std::shared_ptr<Rail>> m_Rails;

public:
    bool m_DrawRails = false;
    std::string m_ControlPointsFileName = "controlPoints";
    std::vector<std::string> m_ControlPointsFiles;
    std::string m_ControlPointsFileSelected = "";

    Material *m_Material_curve;

    VertexArray *m_VAO_tangents;
    VertexBuffer *m_VBO_tangents;
    std::vector<glm::vec3> m_Tangents;

    VertexArray *m_VAO_normals;

    VertexArray *m_VAO_binormals;
    VertexBuffer *m_VBO_binormals;
    std::vector<glm::vec3> m_Binormals;
};

#endif // RAILS_H