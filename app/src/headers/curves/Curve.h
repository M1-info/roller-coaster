#pragma once
#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CurveType
{
    BEZIER,
    BSPLINE,
    CATMULL_ROM,
};

class Curve
{
public:
    Curve();
    Curve(std::vector<glm::vec3> points);
    virtual ~Curve();

    inline void SetPoints(std::vector<glm::vec3> points) { m_Points = points; }

    /*
     return the point on the curve at the given t value
    */
    glm::vec3 GetPoint(float t);

    /*
     return the tangent at the given t value
    */
    glm::vec3 GetTangent(float t, glm::vec3 point);

    /*
     return the normal at the given t value
    */
    glm::vec3 GetNormal(float t, glm::vec3 point, glm::vec3 tangent);
    glm::vec3 GetNormal(glm::vec3 binormal, glm::vec3 tangent);

    /*
     return the binormal at the given t value
    */
    glm::vec3 GetBinormal(float t, glm::vec3 point, glm::vec3 tangent);

protected:
    std::vector<glm::vec3> m_Points;

    glm::mat4 m_Matrix;
};

#endif // CURVE_H