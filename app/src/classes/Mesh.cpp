#include "headers/Mesh.h"

Mesh::Mesh()
    : m_Vertices(), m_Indices(), m_VAO(nullptr), m_VBO(nullptr), m_IBO(nullptr),
      m_Shader(nullptr), m_Matrix(glm::mat4(1.0f))
{
    // unique name
    m_Name = std::to_string(rand());
}

Mesh::Mesh(std::string name, std::vector<float> vertices, std::vector<unsigned int> indices)
    : m_Matrix(glm::mat4(1.0f))
{

    m_Name = name;

    m_Vertices = vertices;
    m_Indices = indices;
    m_Vertices_size = m_Vertices.size() * sizeof(float);
    m_Indices_size = m_Indices.size() * sizeof(unsigned int);

    m_VAO = new VertexArray();
    m_VBO = new VertexBuffer(m_Vertices.data(), m_Vertices_size);

    // create vertex buffer layout
    VertexBufferLayout layout;
    layout.Push<float>(3); // position

    // add vertex buffer layout to vertex array
    m_VAO->AddBuffer(*m_VBO, layout);

    m_IBO = new IndexBuffer(m_Indices.data(), m_Indices_size);

    Clear();
}

Mesh::~Mesh()
{
    m_Vertices.clear();
    m_Indices.clear();

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

void Mesh::SetName(std::string name)
{
    m_Name = name;
}

void Mesh::SetVertices(std::vector<float> vertices)
{
    m_Vertices = vertices;
}

void Mesh::SetIndices(std::vector<unsigned int> indices)
{
    m_Indices = indices;
}

void Mesh::SetMatrix(glm::mat4x4 matrix)
{
    m_Matrix = matrix;
}

void Mesh::SetShader(Shader *shader)
{
    m_Shader = shader;
}

std::string Mesh::GetName() const
{
    return m_Name;
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

std::vector<float> Mesh::GetVertices() const
{
    return m_Vertices;
}

std::vector<unsigned int> Mesh::GetIndices() const
{
    return m_Indices;
}

glm::mat4 Mesh::GetMatrix() const
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
