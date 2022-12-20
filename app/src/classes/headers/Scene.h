#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <string>

#include "Skybox.h"
#include "Mesh.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void Init();
    void Add(Mesh *mesh);
    void Pop(Mesh *mesh);
    void Clear();

    std::vector<Mesh *> GetObjects() const;
    std::map<std::string, Mesh *> GetObjectsMap() const;
    Skybox *GetSkybox() const;

private:
    Skybox *m_Skybox;
    std::vector<Mesh *> m_Objects;
    std::map<std::string, Mesh *> m_Objects_map;
};

#endif // SCENE_H