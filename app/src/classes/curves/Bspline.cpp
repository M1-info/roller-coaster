#include "curves/Bspline.h"

Bspline::Bspline(std::vector<glm::vec3> points)
{
    m_Points = points;
    m_Matrix = glm::transpose(BSPLINE_MATRIX) / 6.0f;
}