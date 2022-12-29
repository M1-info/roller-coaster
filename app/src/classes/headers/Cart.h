#pragma once
#ifndef CART_H
#define CART_H


#include "Mesh.h"

class Cart : public Mesh
{

public:
    Cart(const std::string filename);

    void Draw() override;
    void Update() override {};
    // void UpdateVelocity(float gravity);

// private:
//     glm::vec3 m_Velocity;
};

#endif // CART_H