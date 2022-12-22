#include "headers/Scene.h"

Scene::Scene() : m_Objects(), m_Objects_map()
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

void Scene::Add(Mesh *mesh)
{
    m_Objects.push_back(std::make_shared<Mesh>(*mesh));
    m_Objects_map[mesh->GetName()] = std::make_shared<Mesh>(*mesh);
}

void Scene::Remove(std::shared_ptr<Mesh> mesh)
{
    std::vector<std::shared_ptr<Mesh>>::iterator it = std::find(m_Objects.begin(), m_Objects.end(), mesh);
    if (it != m_Objects.end()){
        m_Objects.erase(it);
        m_Objects_map.erase(mesh->GetName());
    }
    
}

void Scene::Clear()
{
    m_Objects.clear();
    m_Objects_map.clear();
}

std::vector<std::shared_ptr<Mesh>> Scene::GetObjects() const
{
    return m_Objects;
}

std::map<std::string, std::shared_ptr<Mesh>> Scene::GetObjectsMap() const
{
    return m_Objects_map;
}

Skybox *Scene::GetSkybox() const
{
    return m_Skybox;
}