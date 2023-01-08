#include "curves/CatmullRom.h"

CatmullRom::CatmullRom()
{
    m_Matrix = glm::transpose(CATMLLROM_MATRIX) / 2.0f;
}

CatmullRom::CatmullRom(std::vector<glm::vec3> points)
{
    m_Points = points;
    m_Matrix = glm::transpose(CATMLLROM_MATRIX) / 2.0f;
}
