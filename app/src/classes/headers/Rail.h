#pragma once
#ifndef RAIL_H
#define RAIL_H

#include <string>

#include "Mesh.h"
#include "OBJLoader.h"

class Rail : public Mesh
{

public:
    Rail(const std::string filename);

    void SetUpShader();

    void Draw() override;
    void Update() override{};
};

#endif // RAIL_H