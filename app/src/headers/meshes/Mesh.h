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

#include "buffers/VertexArray.h"
#include "buffers/VertexBuffer.h"
#include "buffers/IndexBuffer.h"

#include "utils/Material.h"
#include "utils/Shader.h"
#include "utils/Transform.h"

enum MeshType
{
    MESH,
    RAILS,
    CART,
    CONTROL_POINT,
    RAIL,
    SKYBOX,
    GROUND,
    PLOT
};

class Mesh : public std::enable_shared_from_this<Mesh>
{
public:
    Mesh() = default;
    virtual ~Mesh();

    void SetUpBuffers();
    void SetUp();
    void Clear();

    inline void SetName(std::string name) { m_Name = name; }
    inline void SetVertices(std::vector<glm::vec3> vertices) { m_Vertices = vertices; }
    inline void SetNormals(std::vector<glm::vec3> normals) { m_Normals = normals; }
    inline void SetIndices(std::vector<unsigned int> indices) { m_Indices = indices; }
    inline void SetParent(std::shared_ptr<Mesh> parent) { m_Parent = parent; }
    inline void SetMaterial(Material *material) { m_Material = material; }

    inline VertexArray *GetVAO() const { return m_VAO; }
    inline VertexBuffer *GetVBOPositions() const { return m_VBO_positions; }
    inline VertexBuffer *GetVBONormals() const { return m_VBO_normals; }
    inline IndexBuffer *GetIBO() const { return m_IBO; }
    inline std::vector<glm::vec3> GetVertices() const { return m_Vertices; }
    inline glm::vec3 *GetVertexPtr(int index) { return &m_Vertices[index]; }
    inline std::vector<glm::vec3> GetNormals() const { return m_Normals; }
    inline std::vector<unsigned int> GetIndices() const { return m_Indices; }

    inline std::string GetName() const { return m_Name; }
    inline MeshType GetType() const { return m_Type; }

    inline Material *GetMaterial() const { return m_Material; }
    inline Transform *GetTransform() const { return m_Transform; }

    inline std::shared_ptr<Mesh> GetParent() const { return m_Parent; }
    inline std::vector<std::shared_ptr<Mesh>> GetChildren() const { return m_Children; }

    void AddChildren(std::shared_ptr<Mesh> child);
    virtual void RemoveChildren(std::shared_ptr<Mesh> child);

    void ToggleIsSelected();
    void CreateMaterial(std::string shaderFile);

    virtual void Draw() = 0;
    virtual void Update() = 0;

protected:
    std::string m_Name;
    MeshType m_Type = MeshType::MESH;

    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec3> m_Normals;
    std::vector<unsigned int> m_Indices;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO_positions;
    VertexBuffer *m_VBO_normals;
    IndexBuffer *m_IBO;
    Material *m_Material;

    Transform *m_Transform;

    std::shared_ptr<Mesh> m_Parent;
    std::vector<std::shared_ptr<Mesh>> m_Children;

public:
    bool m_IsSelected = false;
};

#endif // MESH_H