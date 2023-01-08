#pragma once
#ifndef CART_H
#define CART_H

#define GRAVITY 9.81f
#define CART_MASS 1.0f
#define CART_FRICTION 0.1f
#define TIME_STEP 0.01f

#include "Mesh.h"
#include "Object3D.h"
#include "curves/Curve.h"

class Cart : public Mesh
{
public:
    Cart();
    static std::shared_ptr<Cart> Create();

    void Draw() override;
    void Update() override;
    void Animate();

    inline void SetVelocity(glm::vec3 velocity) { m_Velocity = velocity; }
    inline glm::vec3 GetVelocity() { return m_Velocity; }

    inline void SetCurves(std::vector<Curve> curves) { m_Curves = curves; }
    inline void SetCurrentCurve(Curve *currentCurve) { m_CurrentCurve = currentCurve; }
    inline std::vector<Curve> GetCurves() { return m_Curves; }
    inline Curve *GetCurrentCurve() { return m_CurrentCurve; }

    inline void SetCurrentCurveIndex(int currentCurveIndex) { m_CurrentCurveIndex = currentCurveIndex; }
    inline int GetCurrentCurveIndex() { return m_CurrentCurveIndex; }

    inline void SetCurveCount(int curveCount) { m_CurveCount = curveCount; }

    // inline void SetRails(std::vector<glm::vec3> rails) { m_Rails = rails; }
    // inline std::vector<glm::vec3> GetRails() { return m_Rails; };
    // inline void SetTangents(std::vector<glm::vec3> tangents) { m_Tangents = tangents; }
    // inline std::vector<glm::vec3> GetTangents() { return m_Tangents; }

    // inline void SetCurrentRail(std::vector<glm::vec3>::iterator currentRail) { m_CurrentRail = currentRail; }
    // inline void SetCurrentTangent(std::vector<glm::vec3>::iterator currentTangent) { m_CurrentTangent = currentTangent; }

private:
    glm::vec3 m_Velocity;
    glm::vec3 m_Destination;

    std::vector<Curve> m_Curves;
    Curve *m_CurrentCurve;
    int m_CurrentCurveIndex = 0;
    int m_CurveCount = 0;
    float m_CurrentCurveTime = 0.0f;

    // std::vector<glm::vec3> m_Rails;
    // std::vector<glm::vec3> m_Tangents;

    // std::vector<glm::vec3>::iterator m_CurrentRail;
    // std::vector<glm::vec3>::iterator m_CurrentTangent;
};

#endif // CART_H