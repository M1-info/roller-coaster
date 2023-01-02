#pragma once
#ifndef RAIL_H
#define RAIL_H

#include <string>

#include "meshes/Mesh.h"
#include "utils/OBJLoader.h"
#include "Light.h"

class Rail : public Mesh
{

public:
    Rail(const std::string filename, int index);

    void Draw() override;
    void Update() override{};
};

#endif // RAIL_H