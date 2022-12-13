#include "headers/Mesh.h"

Mesh::Mesh()
    : m_Vertices(nullptr), m_Indices(nullptr), m_VAO(nullptr), m_VBO(nullptr), m_IBO(nullptr), m_Shader(nullptr), m_Matrix(glm::mat4(1.0f))
{
}

Mesh::Mesh(float *vertices, unsigned int *indices, const unsigned int vertices_size, const unsigned int indices_size)
{
    m_Vertices = vertices;
    m_Indices = indices;
    m_VAO = new VertexArray();
    m_VBO = new VertexBuffer(m_Vertices, vertices_size);

    // create vertex buffer layout
    VertexBufferLayout layout;
    layout.Push<float>(3); // position

    // add vertex buffer layout to vertex array
    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = new IndexBuffer(m_Indices, indices_size); 

    Clear();
}

Mesh::~Mesh()
{
    delete m_Vertices;
    delete m_Indices;
    delete m_VAO;
    delete m_VBO;
    delete m_IBO;
}

void Mesh::SetUp()
{
    m_VAO->Bind();
    m_VBO->Bind();
    m_IBO->Bind();
    m_Shader->Bind();
}

void Mesh::Clear()
{
    m_VAO->Unbind();
    m_VBO->Unbind();
    m_IBO->Unbind();
    m_Shader->Unbind();
}

void Mesh::SetVertices(float *vertices)
{
    m_Vertices = &vertices[0];
}

void Mesh::SetIndices(unsigned int *indices)
{
    m_Indices = &indices[0];
}

void Mesh::SetMatrix(glm::mat4x4 matrix)
{
    m_Matrix = matrix;
}

void Mesh::SetShader(Shader *shader)
{
    m_Shader = shader;
}

VertexArray *Mesh::GetVAO() const
{
    return m_VAO;
}

VertexBuffer *Mesh::GetVBO() const
{
    return m_VBO;
}

IndexBuffer *Mesh::GetIBO() const
{
    return m_IBO;
}

float *Mesh::GetVertices() const
{
    return m_Vertices;
}

unsigned int *Mesh::GetIndices() const
{
    return m_Indices;
}

glm::mat4x4 Mesh::GetMatrix() const
{
    return m_Matrix;
}

Shader *Mesh::GetShader() const
{
    return m_Shader;
}

void Mesh::Translate(glm::vec3 translation)
{
    m_Matrix = glm::translate(m_Matrix, translation);
}

void Mesh::Rotate(GLfloat angle, glm::vec3 axis)
{
    m_Matrix = glm::rotate(m_Matrix, angle, axis);
}

void Mesh::Scale(glm::vec3 scale)
{
    m_Matrix = glm::scale(m_Matrix, scale);
}

void Mesh::AddShader(std::string filename)
{
    m_Shader = new Shader(filename);
}
