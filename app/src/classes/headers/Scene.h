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
    void Add(Mesh *mesh);
    void Remove(std::shared_ptr<Mesh> mesh);
    void Clear();

    std::vector<std::shared_ptr<Mesh>> GetObjects() const;
    std::map<std::string, std::shared_ptr<Mesh>> GetObjectsMap() const;
    Skybox *GetSkybox() const;

private:
    Skybox *m_Skybox;
    std::vector<std::shared_ptr<Mesh>> m_Objects;
    std::map<std::string, std::shared_ptr<Mesh>> m_Objects_map;
};

#endif // SCENE_H