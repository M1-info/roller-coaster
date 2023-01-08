#pragma once
#ifndef CATMULLROM_H
#define CATMULLROM_H

#define CATMLLROM_MATRIX glm::mat4(-1.0f, 3.0f, -3.0f, 1.0f, 2.0f, -5.0f, 4.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f)

#include "Curve.h"

#include <vector>
#include <glm/glm.hpp>

class CatmullRom : public Curve
{
public:
    CatmullRom(std::vector<glm::vec3> points);
};

#endif // CATMULLROM_H