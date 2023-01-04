#include "curves/CatmullRom.h"

CatmullRom::CatmullRom(std::vector<glm::vec3> points)
{
    m_Points = points;
    m_Matrix = CATMLLROM_MATRIX / 2.0f;
}
