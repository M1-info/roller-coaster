#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>

#include "Skybox.h"
#include "Mesh.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void Init();
    void Add(std::shared_ptr<Mesh> mesh);
    void Remove(std::shared_ptr<Mesh> mesh);
    void Clear();

    std::vector<std::shared_ptr<Mesh>> GetObjects() const;
    Skybox *GetSkybox() const;

private:
    Skybox *m_Skybox;
    std::vector<std::shared_ptr<Mesh>> m_Objects;
};

#endif // SCENE_H