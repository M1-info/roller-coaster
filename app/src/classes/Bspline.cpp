#include "headers\Bspline.h"

Bspline::Bspline()
{
}

Bspline::Bspline(std::vector<glm::vec3> controlPoints)
{
    m_ControlPoints = controlPoints;
}

Bspline::~Bspline()
{
}

std::vector<glm::vec3> Bspline::GetControlPoints()
{
    return m_ControlPoints;
}

glm::vec3 Bspline::GetControlPoint(int index)
{
    return m_ControlPoints[index];
}

glm::vec3 Bspline::GetPoint(float t)
{
    glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec4 T = glm::vec4(t * t * t, t * t, t, 1.0f);

    glm::mat4 M = BSPLINE_CURVE_MATRIX * (1.0f / 6.0f);

    glm::mat3x4 P = glm::mat3x4(
        {m_ControlPoints[0].x, m_ControlPoints[1].x, m_ControlPoints[2].x, m_ControlPoints[3].x},
        {m_ControlPoints[0].y, m_ControlPoints[1].y, m_ControlPoints[2].y, m_ControlPoints[3].y},
        {m_ControlPoints[0].z, m_ControlPoints[1].z, m_ControlPoints[2].z, m_ControlPoints[3].z});

    return T * M * P;
}

glm::vec3 Bspline::GetTangent(float t)
{

    float dx_a = 3 * (m_ControlPoints[1].x - m_ControlPoints[0].x);
    float dx_b = 3 * (m_ControlPoints[2].x - m_ControlPoints[1].x);
    float dx_c = 3 * (m_ControlPoints[3].x - m_ControlPoints[2].x);

    float dx = dx_a * pow(1 - t, 2.0) + 2 * dx_b * (1 - t) * t + dx_c * pow(t, 2.0);

    float dy_a = 3 * (m_ControlPoints[1].y - m_ControlPoints[0].y);
    float dy_b = 3 * (m_ControlPoints[2].y - m_ControlPoints[1].y);
    float dy_c = 3 * (m_ControlPoints[3].y - m_ControlPoints[2].y);

    float dy = dy_a * pow(1 - t, 2.0) + 2 * dy_b * (1 - t) * t + dy_c * pow(t, 2.0);

    float dz_a = 3 * (m_ControlPoints[1].z - m_ControlPoints[0].z);
    float dz_b = 3 * (m_ControlPoints[2].z - m_ControlPoints[1].z);
    float dz_c = 3 * (m_ControlPoints[3].z - m_ControlPoints[2].z);

    float dz = dz_a * pow(1 - t, 2.0) + 2 * dz_b * (1 - t) * t + dz_c * pow(t, 2.0);

    return glm::vec3(dx, dy, dz);
}

glm::vec3 Bspline::GetNormalisedTangent(float t, glm::vec3 point, glm::vec3 tangent)
{
    glm::vec3 point2 = GetPoint(t + 0.001);

    glm::vec3 tangent2 = GetTangent(t + 0.001);

    tangent2 += point - point2;

    tangent2 = glm::normalize(tangent2);

    return glm::cross(tangent, tangent2);
}