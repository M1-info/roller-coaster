#pragma once
#ifndef GROUND_H
#define GROUND_H

#include <glm/glm.hpp>
#include "Mesh.h"

class Ground : public Mesh
{
public:
    Ground(const std::string filename);

    void Draw() override;
    void Update() override{};
};

#endif // GROUND_H