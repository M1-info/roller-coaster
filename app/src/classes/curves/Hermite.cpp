#include "curves/Hermite.h"

Hermite::Hermite(std::vector<glm::vec3> points)
{
    m_Points = points;
    m_Matrix = HERMITE_MATRIX;
};