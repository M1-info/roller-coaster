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

    void SetControlPoints(std::vector<glm::vec3> controlPoints);
    void AddControlPoint(glm::vec3 controlPoint);
    void RemoveControlPoint(int index);
    void ClearControlPoints();

    std::vector<glm::vec3> GetControlPoints();
    glm::vec3 GetControlPoint(int index);

    glm::vec3 GetPoint(float t);
    std::vector<glm::vec3> GetPoints(int resolution);

private:
    std::vector<glm::vec3> m_ControlPoints;
};


#endif // BEZIERCURVE_H