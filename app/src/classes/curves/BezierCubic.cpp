#include "curves/BezierCubic.h"

BezierCubic::BezierCubic()
{
    m_Matrix = BEZIER_CUBIC_MATRIX;
}

BezierCubic::BezierCubic(std::vector<glm::vec3> points)
{
    m_Points = points;
    m_Matrix = BEZIER_CUBIC_MATRIX;
}