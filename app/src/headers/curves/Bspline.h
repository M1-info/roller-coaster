#pragma once
#ifndef BSPLINE_H
#define BSPLINE_H

#define BSPLINE_MATRIX glm::mat4(-1, 3, -3, 1, 3, -6, 3, 0, -3, 0, 3, 0, 1, 4, 1, 0)

#include "Curve.h"
#include "CatmullRom.h"
#include "BezierCubic.h"

#include <vector>
#include <glm/glm.hpp>

class Bspline : public Curve
{
public:
    Bspline();
    Bspline(std::vector<glm::vec3> points);
};

#endif // BSPLINE_H