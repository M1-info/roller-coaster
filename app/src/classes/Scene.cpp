#include "Scene.h"

Scene::Scene() : m_Objects(), m_Skybox(nullptr)
{
}

Scene::~Scene()
{
    delete m_Skybox;
    m_Skybox = nullptr;

    m_Objects.clear();
}

void Scene::Init()
{

    m_Skybox = new Skybox();
    m_Skybox->Init();

    std::shared_ptr<Cart> cart = std::make_shared<Cart>("cart.obj");
    std::shared_ptr<Rails> rails = Rails::Create();

    Add(cart);
    Add(rails);
}

void Scene::SetUpObjectsShaders(std::shared_ptr<Light> light)
{
    for (auto mesh : m_Objects)
    {
        // rails don't need uniforms about light and material
        if (mesh->GetType() == MeshType::RAILS)
        {
            std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(mesh);
            for (auto rail : rails->GetRails())
                rail->GetMaterial()->SetUpShader(light);

            continue;
        }

        mesh->GetMaterial()->SetUpShader(light);
    }
}

void Scene::Add(std::shared_ptr<Mesh> object)
{
    m_Objects.push_back(object);
}

void Scene::Remove(std::shared_ptr<Mesh> object)
{
    std::vector<std::shared_ptr<Mesh>>::iterator it = std::find(m_Objects.begin(), m_Objects.end(), object);
    if (it != m_Objects.end())
    {
        m_Objects.erase(it);
    }
}

void Scene::Clear()
{
    m_Objects.clear();
}

std::vector<std::shared_ptr<Mesh>> Scene::GetObjects() const
{
    return m_Objects;
}

std::shared_ptr<Mesh> Scene::GetObjectByName(const std::string name) const
{

    auto it = std::find_if(m_Objects.begin(), m_Objects.end(), [&name](std::shared_ptr<Mesh> mesh)
                           { return mesh->GetName() == name; });

    if (it != m_Objects.end())
        return *it;

    return nullptr;
}

std::vector<std::shared_ptr<Mesh>> Scene::GetObjectsByType(const MeshType type) const
{
    std::vector<std::shared_ptr<Mesh>> objects;

    for (auto mesh : m_Objects)
    {
        if (mesh->GetType() == type)
            objects.push_back(mesh);
    }

    return objects;
}