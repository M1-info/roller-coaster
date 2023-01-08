#include "curves/Curve.h"

Curve::Curve()
{
}

Curve::Curve(std::vector<glm::vec3> points)
{
    m_Points = points;
}

Curve::~Curve()
{
}

glm::vec3 Curve::GetPoint(float t)
{
    // T Matrix with t^3, t^2, t, 1
    glm::vec4 T = glm::vec4(t * t * t, t * t, t, 1.0f);

    // M Matrix based on bernstein polynomials
    glm::mat4 M = m_Matrix;

    // P Matrix with the control points
    glm::mat3x4 P = glm::mat3x4(
        {m_Points[0].x, m_Points[1].x, m_Points[2].x, m_Points[3].x},
        {m_Points[0].y, m_Points[1].y, m_Points[2].y, m_Points[3].y},
        {m_Points[0].z, m_Points[1].z, m_Points[2].z, m_Points[3].z});

    return T * M * P;
}

glm::vec3 Curve::GetTangent(float t)
{
    float dx_a = 3 * (m_Points[1].x - m_Points[0].x);
    float dx_b = 3 * (m_Points[2].x - m_Points[1].x);
    float dx_c = 3 * (m_Points[3].x - m_Points[2].x);

    float dx = dx_a * pow(1 - t, 2.0) + 2 * dx_b * (1 - t) * t + dx_c * pow(t, 2.0);

    float dy_a = 3 * (m_Points[1].y - m_Points[0].y);
    float dy_b = 3 * (m_Points[2].y - m_Points[1].y);
    float dy_c = 3 * (m_Points[3].y - m_Points[2].y);

    float dy = dy_a * pow(1 - t, 2.0) + 2 * dy_b * (1 - t) * t + dy_c * pow(t, 2.0);

    float dz_a = 3 * (m_Points[1].z - m_Points[0].z);
    float dz_b = 3 * (m_Points[2].z - m_Points[1].z);
    float dz_c = 3 * (m_Points[3].z - m_Points[2].z);

    float dz = dz_a * pow(1 - t, 2.0) + 2 * dz_b * (1 - t) * t + dz_c * pow(t, 2.0);

    return glm::vec3(dx, dy, dz);
}

glm::vec3 Curve::GetNormal(float t, glm::vec3 point, glm::vec3 tangent)
{

    glm::vec3 binormal = GetBinormal(t, point, tangent);

    return glm::cross(binormal, glm::normalize(tangent));
}

glm::vec3 Curve::GetNormal(glm::vec3 binormal, glm::vec3 tangent)
{
    return glm::cross(binormal, glm::normalize(tangent));
}

glm::vec3 Curve::GetBinormal(float t, glm::vec3 point, glm::vec3 tangent)
{
    glm::vec3 nearPoint = GetPoint(t + 0.001);
    glm::vec3 nearPointTangent = GetTangent(t + 0.001);

    glm::vec3 unitTangent = glm::normalize(tangent);

    glm::vec3 binormal = glm::cross(unitTangent, nearPointTangent) / glm::length(glm::cross(unitTangent, nearPointTangent));

    return binormal;
}
