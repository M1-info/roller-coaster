#pragma once
#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#define BEZIER_CURVE_MATRIX glm::mat4(-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0)


#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class BezierCurve
{
public:
    BezierCurve();
    BezierCurve(std::vector<glm::vec3> controlPoints);
    ~BezierCurve();

    /* return the control points of the curve */
    std::vector<glm::vec3> GetControlPoints();

    /* return the control point at the given index */
    glm::vec3 GetControlPoint(int index);

    /* t is a value between 0 and 1
     0 is the start of the curve
     1 is the end of the curve
     return the point on the curve at the given t value
    */
    glm::vec3 GetPoint(float t);

private:
    std::vector<glm::vec3> m_ControlPoints;
};


#endif // BEZIERCURVE_H