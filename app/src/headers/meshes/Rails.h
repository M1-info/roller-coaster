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

#include "curves/Curve.h"
#include "curves/BezierCubic.h"
#include "curves/Bspline.h"
#include "curves/CatmullRom.h"

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
    void UpdateControlPoints();

    void GenerateControlPoints(std::vector<glm::vec3> controlPoints);

    inline std::vector<std::shared_ptr<Rail>> GetRails() { return m_Rails; }
    inline std::vector<glm::vec3> GetTangents() { return m_Tangents; }

private:
    std::vector<std::shared_ptr<Rail>> m_Rails;
    std::vector<glm::vec3> m_Tangents;

public:
    bool m_DrawRails = false;
    CurveType m_CurveType = CurveType::BEZIER;
};

#endif // RAILS_H