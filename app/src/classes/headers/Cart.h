#pragma once
#ifndef CART_H
#define CART_H


#include "Mesh.h"

class Cart : public Mesh
{

public:
    Cart(const std::string filename);

    void Draw() override;
};

#endif // CART_H