#ifndef MESH_H
#define MESH_H

#include <string>

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

class Mesh
{
public:
    Mesh();
    Mesh(std::string name, std::vector<Vertex> vertices, std::vector<IndexesFace> indices, std::vector<Normal> normales);

    static Mesh * FromOBJ(const std::string path);
    ~Mesh();

    void SetUp();
    void Clear();

    void SetName(std::string name);
    void SetVertices(std::vector<Vertex> vertices);
    void SetNormales(std::vector<Normal> normales);
    void SetIndices(std::vector<IndexesFace> indices);
    void CreateMaterial(std::string shaderFile);

    VertexArray *GetVAO() const;
    VertexBuffer *GetVBO() const;
    IndexBuffer *GetIBO() const;
    glm::mat4 GetMatrix() const;
    Material *GetMaterial() const;

    std::string GetName() const;
    std::vector<Vertex> GetVertices() const;
    std::vector<Normal> GetNormales() const;
    std::vector<IndexesFace> GetIndices() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetRotation() const;

    void Translate(glm::vec3 translation);
    void Rotate(GLfloat angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);
    glm::mat4 ComputeMatrix();

private:
    std::string m_Name;
    std::vector<Vertex> m_Vertices;
    std::vector<Normal> m_Normales;
    std::vector <TextureCoordinate> m_TexCoords;
    std::vector<IndexesFace> m_Indices;
    std::vector<unsigned int> m_Indices_old;

    size_t m_Vertices_size;
    size_t m_Normales_size;
    size_t m_TexCoords_size;
    size_t m_Indices_size;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO_pos;
    VertexBuffer *m_VBO_norm;
    VertexBuffer *m_VBO_tex;
    IndexBuffer *m_IBO;
    Material *m_Material;

    glm::mat4 m_Matrix;

public: 
   glm::vec3 m_Position;
   glm::vec3 m_Scale;
   glm::vec3 m_Rotation;
};

#endif // MESH_H