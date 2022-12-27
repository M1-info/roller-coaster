#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <memory>

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

enum MeshType {
    MESH,
    RAILS,
    CART,
    CONTROL_POINT
};

class Mesh
{
public:
    Mesh() = default;
    Mesh(std::string name, std::vector<glm::vec3> vertices, std::vector<IndexesFace> indices, std::vector<glm::vec3> normales);
    virtual ~Mesh();

    void SetUp();
    void Clear();

    void SetName(std::string name);
    void SetVertices(std::vector<glm::vec3> vertices);
    void SetNormales(std::vector<glm::vec3> normales);
    void SetIndices(std::vector<IndexesFace> indices);
    void CreateMaterial(std::string shaderFile);
    void SetParent(std::shared_ptr<Mesh> parent);
    void SetChildren(std::vector<std::shared_ptr<Mesh>>);

    VertexArray *GetVAO() const;
    VertexBuffer *GetVBO() const;
    IndexBuffer *GetIBO() const;
    glm::mat4 GetMatrix() const;
    Material *GetMaterial() const;

    std::string GetName() const;
    std::vector<glm::vec3> GetVertices() const;
    glm::vec3 *GetVertexPtr(int index);
    std::vector<glm::vec3> GetNormales() const;
    std::vector<IndexesFace> GetIndices() const;
    std::shared_ptr<Mesh> GetParent() const;
    std::vector<std::shared_ptr<Mesh>> GetChildren() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetRotation() const;
    MeshType GetType() const;

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
    std::vector<IndexesFace> m_Indices;

    size_t m_Vertices_size;
    size_t m_Normales_size;
    size_t m_Indices_size;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO_pos;
    VertexBuffer *m_VBO_norm;
    IndexBuffer *m_IBO;
    Material *m_Material;

    glm::mat4 m_Matrix;

    std::shared_ptr<Mesh> m_Parent;
    std::vector<std::shared_ptr<Mesh>> m_Children;

    MeshType m_Type = MeshType::MESH; 

public: 
   glm::vec3 m_Position;
   glm::vec3 m_Scale;
   glm::vec3 m_Rotation;
};

#endif // MESH_H