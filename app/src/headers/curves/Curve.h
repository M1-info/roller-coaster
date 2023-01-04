#pragma once
#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Curve
{
public:
    Curve();
    Curve(std::vector<glm::vec3> points);
    virtual ~Curve();

    /* t is a value between 0 and 1
     0 is the start of the curve
     1 is the end of the curve
     return the point on the curve at the given t value
    */
    glm::vec3 GetPoint(float t);

    /* t is a value between 0 and 1
     0 is the start of the curve
     1 is the end of the curve
     return the tangent at the given t value
    */
    glm::vec3 GetTangent(float t);

    /* t is a value between 0 and 1
     0 is the start of the curve
     1 is the end of the curve
     return the tangent normalised at the given t value
    */
    glm::vec3 GetNormalisedTangent(float t, glm::vec3 point, glm::vec3 tangent);

protected:
    std::vector<glm::vec3> m_Points;

    glm::mat4 m_Matrix;
};

#endif // CURVE_H