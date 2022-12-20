#include "headers/Scene.h"

Scene::Scene() : m_Objects(), m_Objects_map()
{
}

Scene::~Scene()
{
}

void Scene::init()
{
}

void Scene::Add(Mesh *mesh)
{
    m_Objects.push_back(mesh);
    m_Objects_map[mesh->GetName()] = mesh;
}

void Scene::Pop(Mesh *mesh)
{
    m_Objects.pop_back();
    m_Objects_map.erase(mesh->GetName());
}

void Scene::Clear()
{
    m_Objects.clear();
    m_Objects_map.clear();
}

std::vector<Mesh *> Scene::GetObjects() const
{
    return m_Objects;
}

std::map<std::string, Mesh *> Scene::GetObjectsMap() const
{
    return m_Objects_map;
}