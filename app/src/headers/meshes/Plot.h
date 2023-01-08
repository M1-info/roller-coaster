#pragma once
#ifndef PLOT_H
#define PLOT_H

#define PLOT_SIZE 0.5f

#include "Mesh.h"
#include "Object3D.h"
#include "utils/OBJLoader.h"

class Plot : public Mesh
{
public:
    Plot();
    static std::shared_ptr<Plot> Create(int index, Transform *parent_transform);

    void Draw() override;
    void Update() override;
};

#endif // PLOT_H