#pragma once
#ifndef HERMITE_H
#define HERMITE_H

#define HERMITE_MATRIX glm::mat4(2.0f, -2.0f, 1.0f, 1.0f, -3.0f, 3.0f, -2.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f)

#include "Curve.h"

#include <vector>
#include <glm/glm.hpp>

class Hermite : public Curve
{
public:
    Hermite(std::vector<glm::vec3> points);
};

#endif // HERMITE_H