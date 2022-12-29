#include "headers/Scene.h"

Scene::Scene() : m_Objects()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
    m_Skybox = new Skybox();
    m_Skybox->Init();
}

void Scene::Add(std::shared_ptr<Mesh> mesh)
{
    m_Objects.push_back(mesh);
}

void Scene::Remove(std::shared_ptr<Mesh> mesh)
{
    std::vector<std::shared_ptr<Mesh>>::iterator it = std::find(m_Objects.begin(), m_Objects.end(), mesh);
    if (it != m_Objects.end())
        m_Objects.erase(it);
}

void Scene::Clear()
{
    m_Objects.clear();
}

std::vector<std::shared_ptr<Mesh>> Scene::GetObjects() const
{
    return m_Objects;
}

Skybox *Scene::GetSkybox() const
{
    return m_Skybox;
}