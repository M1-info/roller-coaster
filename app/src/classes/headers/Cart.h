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

    void SetRailsVertices(std::vector<glm::vec3> railsVertices);
    inline std::vector<glm::vec3> GetRailsVertices() { return m_RailsVertices; }

    void SetRailsTangents(std::vector<glm::vec3> railsTangents);
    inline std::vector<glm::vec3> GetRailsTangents() { return m_RailsTangents; }

    void SetRailsNormals(std::vector<glm::vec3> railsNormals);
    inline std::vector<glm::vec3> GetRailsNormals() { return m_RailsNormals; }

    void SetRailsBinormals(std::vector<glm::vec3> railsBinormals);
    inline std::vector<glm::vec3> GetRailsBinormals() { return m_RailsBinormals; }

private:
    glm::vec3 m_Velocity;
    std::vector<glm::vec3> m_RailsVertices;
    std::vector<glm::vec3> m_RailsTangents;
    std::vector<glm::vec3> m_RailsNormals;
    std::vector<glm::vec3> m_RailsBinormals;
    std::vector<glm::vec3>::iterator m_CurrentRailVertex;
    std::vector<glm::vec3>::iterator m_CurrentRailTangent;
    std::vector<glm::vec3>::iterator m_CurrentRailNormal;
    std::vector<glm::vec3>::iterator m_CurrentRailBinormal;
};

#endif // CART_H