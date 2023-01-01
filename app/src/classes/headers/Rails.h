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

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BezierCurve.h"
#include "Bspline.h"
#include "Mesh.h"
#include "Shader.h"
#include "ControlPoint.h"
#include "OBJLoader.h"
#include "Rail.h"

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
    bool ExportRails();
    void LoadRails(const std::string filename);
    void LoadControlPointsFiles();
    bool DeleteFileRails(const std::string filename);

    inline std::vector<std::shared_ptr<Rail>> GetRails() { return m_Rails; }
    inline std::vector<glm::vec3> GetVerticesTangents() { return m_VerticesTangents; }

private:
    std::vector<std::shared_ptr<Rail>> m_Rails;

public:
    bool m_DrawRails = false;
    std::string m_ControlPointsFileName = "controlPoints";
    std::vector<std::string> m_ControlPointsFiles;
    std::string m_ControlPointsFileSelected = "";

    VertexArray *m_VAO_tang;
    VertexBuffer *m_VBO_tang;
    Material *m_Material_tang;
    std::vector<glm::vec3> m_VerticesTangents;

    VertexArray *m_VAO_finalTang;
    VertexBuffer *m_VBO_finalTang;
    Material *m_Material_finalTang;
    std::vector<glm::vec3> m_VerticesFinalTangents;

    VertexArray *m_VAO_norm;
    Material *m_Material_norm;
    std::vector<glm::vec3> m_VerticesNormals;

    VertexArray *m_VAO_binormal;
    VertexBuffer *m_VBO_binormal;
    Material *m_Material_binormal;
    std::vector<glm::vec3> m_VerticesBinormals;
};

#endif // RAILS_H