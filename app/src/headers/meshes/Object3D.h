#pragma once
#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Mesh.h"

/**
 * @brief The Object3D class
 * Utility for 3D objects with no special properties
 */
class Object3D : public Mesh
{
public:
    Object3D();
    Object3D(const std::string name);

    void Draw() override;
    void Update() override;
};

#endif // OBJECT3D_H