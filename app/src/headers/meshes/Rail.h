#pragma once
#ifndef RAIL_H
#define RAIL_H

#include <string>

#include "Mesh.h"
#include "Plot.h"
#include "Object3D.h"
#include "utils/OBJLoader.h"

class Rail : public Mesh
{

public:
    Rail(int index);
    static std::shared_ptr<Rail> Create(int index);

    void GeneratePlots(int index);

    void Draw() override;
    void Update() override;
};

#endif // RAIL_H