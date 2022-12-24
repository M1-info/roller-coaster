#pragma once
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <string>
#include <glm/glm.hpp>

class SceneObject
{
public: 
    virtual ~SceneObject() {};
    virtual std::string GetName() const {return m_Name;};

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

protected: 
    std::string m_Name;
};


#endif // SCENEOBJECT_H