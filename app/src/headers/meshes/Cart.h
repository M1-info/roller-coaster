#pragma once
#ifndef CART_H
#define CART_H

#include "Mesh.h"

class Cart : public Mesh
{
public:
    Cart(const std::string filename);

    void Draw() override;
    void Update() override{};
    void Animate(float deltaTime);

    inline void SetVelocity(glm::vec3 velocity) { m_Velocity = velocity; }
    inline glm::vec3 GetVelocity() { return m_Velocity; }

    inline void SetRails(std::vector<glm::vec3> rails) { m_Rails = rails; }
    inline std::vector<glm::vec3> GetRails() { return m_Rails; };
    inline void SetTangents(std::vector<glm::vec3> tangents) { m_Tangents = tangents; }
    inline std::vector<glm::vec3> GetTangents() { return m_Tangents; }

    inline void SetCurrentRail(std::vector<glm::vec3>::iterator currentRail) { m_CurrentRail = currentRail; }
    inline void SetCurrentTangent(std::vector<glm::vec3>::iterator currentTangent) { m_CurrentTangent = currentTangent; }

private:
    glm::vec3 m_Velocity;

    std::vector<glm::vec3> m_Rails;
    std::vector<glm::vec3> m_Tangents;

    std::vector<glm::vec3>::iterator m_CurrentRail;
    std::vector<glm::vec3>::iterator m_CurrentTangent;
};

#endif // CART_H