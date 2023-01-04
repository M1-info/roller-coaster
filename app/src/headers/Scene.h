#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>

#include "Camera.h"
#include "Light.h"

#include "meshes/Skybox.h"
#include "meshes/Rails.h"
#include "meshes/Cart.h"
#include "meshes/Ground.h"

class Scene
{
public:
    Scene();
    ~Scene();

    /* GETTERS */
    std::vector<std::shared_ptr<Mesh>> GetObjects() const;
    std::shared_ptr<Mesh> GetObjectByName(const std::string name) const;
    std::vector<std::shared_ptr<Mesh>> GetObjectsByType(const MeshType type) const;
    inline Skybox *GetSkybox() const { return m_Skybox; }

    /**
     * @brief Initialize the scene -
     * Create the skybox and the objects of the scene
     */
    void Init();

    /**
     * @brief Set up the shaders of the objects of the scene
     * @param camera The camera of the scene
     * @param light The light of the scene
     */
    void SetUpObjectsShaders(std::shared_ptr<Light> light);

    /**
     * @brief Add an object to the scene
     * @param object The object to add
     */
    void Add(std::shared_ptr<Mesh> object);

    /**
     * @brief Remove an object from the scene
     * @param object The object to remove
     */
    void Remove(std::shared_ptr<Mesh> object);

    /**
     * @brief Remove all the objects from the scene
     */
    void Clear();

private:
    /**
     * @brief The skybox of the scene, it's a cube with a texture
     * @note That object is not added in the m_Objects vector because it nned a different treatment
     *
     */
    Skybox *m_Skybox;

    /**
     * @brief The objects of the scene
     * @note The objects are stored in a vector because we need to iterate over them
     */
    std::vector<std::shared_ptr<Mesh>> m_Objects;
};

#endif // SCENE_H