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
    Mesh(float *vertices, unsigned int *indices, const unsigned int vertices_size, const unsigned int indices_size);
    ~Mesh();

    void SetUp();
    void Render();
    void Clear();

    void SetVertices(float *vertices);
    void SetIndices(unsigned int *indices);
    void SetMatrix(glm::mat4x4 matrix);
    void SetShader(Shader *shader);

    VertexArray *GetVAO() const;
    VertexBuffer *GetVBO() const;
    IndexBuffer *GetIBO() const;
    float *GetVertices() const;
    unsigned int *GetIndices() const;
    glm::mat4x4 GetMatrix() const;
    Shader *GetShader() const;

    void Translate(glm::vec3 translation);
    void Rotate(GLfloat angle, glm::vec3 axis);
    void Scale(glm::vec3 scale);
    void AddShader(std::string filename);

private:
    float *m_Vertices;
    unsigned int *m_Indices;

    VertexArray *m_VAO;
    VertexBuffer *m_VBO;
    IndexBuffer *m_IBO;
    Shader *m_Shader;

    glm::mat4x4 m_Matrix;
};

#endif // MESH_H