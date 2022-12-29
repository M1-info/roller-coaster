#include "headers/BezierCurve.h"
#include <iostream>

BezierCurve::BezierCurve()
{
}

BezierCurve::BezierCurve(std::vector<glm::vec3> controlPoints)
{
    m_ControlPoints = controlPoints;
}

BezierCurve::~BezierCurve()
{
}

std::vector<glm::vec3> BezierCurve::GetControlPoints()
{
    return m_ControlPoints;
}

glm::vec3 BezierCurve::GetControlPoint(int index)
{
    return m_ControlPoints[index];
}

/**
 * @brief Get the point on the curve at the given t
 * 
 * @param t in [0, 1]
 * @return glm::vec3 
 */
glm::vec3 BezierCurve::GetPoint(float t)
{
    // T Matrix with t^3, t^2, t, 1
    glm::vec4 T = glm::vec4(t * t * t, t * t,  t, 1.0f);

    // M Matrix based on bernstein polynomials
    glm::mat4 M = BEZIER_CURVE_MATRIX;

    // P Matrix with the control points
    glm::mat3x4 P = glm::mat3x4(
        {m_ControlPoints[0].x, m_ControlPoints[1].x, m_ControlPoints[2].x, m_ControlPoints[3].x},
        {m_ControlPoints[0].y, m_ControlPoints[1].y, m_ControlPoints[2].y, m_ControlPoints[3].y},
        {m_ControlPoints[0].z, m_ControlPoints[1].z, m_ControlPoints[2].z, m_ControlPoints[3].z}
    );

    return T * M * P;
}