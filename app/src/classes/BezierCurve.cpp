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

void BezierCurve::SetControlPoints(std::vector<glm::vec3> controlPoints)
{
    m_ControlPoints = controlPoints;
}

void BezierCurve::AddControlPoint(glm::vec3 controlPoint)
{
    m_ControlPoints.push_back(controlPoint);
}

void BezierCurve::RemoveControlPoint(int index)
{
    m_ControlPoints.erase(m_ControlPoints.begin() + index);
}

void BezierCurve::ClearControlPoints()
{
    m_ControlPoints.clear();
}

std::vector<glm::vec3> BezierCurve::GetControlPoints()
{
    return m_ControlPoints;
}

glm::vec3 BezierCurve::GetControlPoint(int index)
{
    return m_ControlPoints[index];
}

glm::vec3 BezierCurve::GetPoint(float t)
{

    glm::vec4 T = glm::vec4(t * t * t, t * t,  t, 1.0f);
    glm::mat4 M = BEZIER_CURVE_MATRIX;
    glm::mat3x4 P = glm::mat3x4(
        {m_ControlPoints[0].x, m_ControlPoints[1].x, m_ControlPoints[2].x, m_ControlPoints[3].x},
        {m_ControlPoints[0].y, m_ControlPoints[1].y, m_ControlPoints[2].y, m_ControlPoints[3].y},
        {m_ControlPoints[0].z, m_ControlPoints[1].z, m_ControlPoints[2].z, m_ControlPoints[3].z}
    );

    return T * M * P;
}

std::vector<glm::vec3> BezierCurve::GetPoints(int resolution)
{
    std::vector<glm::vec3> points;
    for (int i = 0; i < resolution; i++)
    {
        points.push_back(GetPoint((float)i / (float)resolution));
    }
    return points;
}