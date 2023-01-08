#pragma once
#ifndef BEZIERCUBIC_H
#define BEZIERCUBIC_H

#define BEZIER_CUBIC_MATRIX glm::mat4(-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0)

#include "Curve.h"
#include "CatmullRom.h"
#include "Bspline.h"

#include <vector>
#include <glm/glm.hpp>

class BezierCubic : public Curve
{
public:
    BezierCubic();
    BezierCubic(std::vector<glm::vec3> points);
};

#endif // BEZIERCUBIC_H