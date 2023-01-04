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

private:
    glm::vec3 m_Velocity;
};

#endif // CART_H