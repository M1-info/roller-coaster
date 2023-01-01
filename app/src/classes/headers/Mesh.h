#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "VertexArray.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "OBJLoader.h"

enum MeshType
{
    MESH,
    RAILS,
    CART,
    CONTROL_POINT,
    RAIL
};

class Mesh : public std::enable_shared_from_this<Mesh>
{
public:
    Mesh() = default;
    virtual ~Mesh();

    void SetUp();
    void Clear();

    inline void SetName(std::string name) { m_Name = name; }
    inline void SetNormales(std::vector<glm::vec3> normales) { m_Normales = normales; }
    inline void SetIndices(std::vector<unsigned int> indices) { m_Indices = indices; }
    inline void SetParent(std::shared_ptr<Mesh> parent) { m_Parent = parent; }

    inline void SetPosition(glm::vec3 position)
    {
        m_Position = position;
        m_IsDirty = true;
    }
    inline void SetScale(glm::vec3 scale)
    {
        m_Scale = scale;
        m_IsDirty = true;
    }
    inline void SetRotation(glm::vec3 rotation)
    {
        m_Rotation = rotation;
        m_IsDirty = true;
    }

    inline std::string GetName() const { return m_Name; }

    inline VertexArray *GetVAO() const { return m_VAO; }
    inline VertexBuffer *GetVBO() const { return m_VBO_pos; }
    inline IndexBuffer *GetIBO() const { return m_IBO; }
    inline Material *GetMaterial() const { return m_Material; }

    inline glm::vec3 GetPosition() const { return m_Position; }
    inline glm::vec3 GetScale() const { return m_Scale; }
    inline glm::vec3 GetRotation() const { return m_Rotation; }
    inline glm::vec3 *GetPositionPtr() { return &m_Position; }
    inline glm::vec3 *GetScalePtr() { return &m_Scale; }
    inline glm::vec3 *GetRotationPtr() { return &m_Rotation; }
    inline glm::mat4 GetMatrix() const { return m_Matrix; }

    inline std::vector<glm::vec3> GetVertices() const { return m_Vertices; }
    inline glm::vec3 *GetVertexPtr(int index) { return &m_Vertices[index]; }
    inline std::vector<glm::vec3> GetNormales() const { return m_Normales; }
    inline std::vector<unsigned int> GetIndices() const { return m_Indices; }
    inline std::shared_ptr<Mesh> GetParent() const { return m_Parent; }
    inline std::vector<std::shared_ptr<Mesh>> GetChildren() const { return m_Children; }
    inline MeshType GetType() const { return m_Type; }

    void AddChildren(std::shared_ptr<Mesh> child);
    virtual void RemoveChildren(std::shared_ptr<Mesh> child);

    void CreateMaterial(std::string shaderFile);

    void Translate(glm::vec3 translation);
    void Rotate(GLfloat angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);
    glm::mat4 ComputeMatrix();

    virtual void Draw() = 0;
    virtual void Update() = 0;

protected:
    std::string m_Name;
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec3> m_Normales;
    std::vector<unsigned int> m_Indices;

    size_t m_Vertices_size;
    size_t m_Normales_size;
    size_t m_Indices_size;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO_pos;
    VertexBuffer *m_VBO_norm;
    IndexBuffer *m_IBO;
    Material *m_Material;

    glm::vec3 m_Position;
    glm::vec3 m_Scale;
    glm::vec3 m_Rotation;
    glm::mat4 m_Matrix;

    std::shared_ptr<Mesh> m_Parent;
    std::vector<std::shared_ptr<Mesh>> m_Children;

    MeshType m_Type = MeshType::MESH;

public:
    bool m_IsDirty = false;
    bool m_IsSelected = false;
};

#endif // MESH_H