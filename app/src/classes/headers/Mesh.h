#ifndef MESH_H
#define MESH_H

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Mesh
{
public:
    Mesh();
    Mesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void SetUp();
    void Clear();

    void SetName(std::string name);
    void SetVertices(std::vector<float> vertices);
    void SetIndices(std::vector<unsigned int> indices);
    void SetMatrix(glm::mat4 matrix);
    void SetShader(Shader *shader);

    std::string GetName() const;
    VertexArray *GetVAO() const;
    VertexBuffer *GetVBO() const;
    IndexBuffer *GetIBO() const;
    std::vector<float> GetVertices() const;
    std::vector<unsigned int> GetIndices() const;
    glm::mat4 GetMatrix() const;
    Shader *GetShader() const;
    void AddShader(std::string shader_name);

    void Translate(glm::vec3 translation);
    void Rotate(GLfloat angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);

private:
    std::string m_Name;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;

    size_t m_Vertices_size;
    size_t m_Indices_size;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    IndexBuffer *m_IBO;
    Shader *m_Shader;

    glm::mat4x4 m_Matrix;
};

#endif // MESH_H