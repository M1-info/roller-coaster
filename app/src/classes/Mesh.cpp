#include "headers/Mesh.h"

Mesh::Mesh()
    : m_Vertices(), m_Indices(), 
      m_VAO(nullptr), m_VBO_pos(nullptr), m_IBO(nullptr), 
      m_Matrix(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f))
{
    // unique name
    m_Name = std::to_string(rand());
}

Mesh::Mesh(std::string name, std::vector<Vertex> vertices, std::vector<IndexesFace> indices, std::vector<Normal> normales)
    : m_Matrix(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), m_Rotation(glm::vec3(0.0f))
{
     m_Name = name;

    m_Vertices = vertices;
    m_Normales = normales;
    m_Indices = indices;
    m_Vertices_size = m_Vertices.size() * sizeof(Vertex);
    m_Normales_size = m_Normales.size() * sizeof(Normal);
    m_Indices_size = m_Indices.size() * sizeof(IndexesFace);

    // create vertex array
    m_VAO = new VertexArray();

    // create vertex buffer for positions
    m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices_size);
    VertexBufferLayout layout_pos;
    layout_pos.Push<float>(3); // position

    m_VAO->AddBuffer(*m_VBO_pos, layout_pos);

    // create vertex buffer for textures
    // m_VBO_tex = new VertexBuffer(m_TexCoords.data(), m_TexCoords_size);
    // VertexBufferLayout layout_tex;
    // layout_tex.Push<float>(2); // textures

    // m_VAO->AddBuffer(*m_VBO_tex, layout_tex);

    // create vertex buffer for normales
    m_VBO_norm = new VertexBuffer(m_Normales.data(), m_Normales_size);
    VertexBufferLayout layout_norm;
    layout_norm.Push<float>(3); // normales
    
    m_VAO->AddBuffer(*m_VBO_norm, layout_norm, 1);

    // create index buffer
    m_IBO = new IndexBuffer(m_Indices.data(), m_Indices_size);

    Clear();
}

Mesh *Mesh::FromOBJ(const std::string filename)
{
    OBJLoader loader(filename);

    std::vector<Vertex> vertices = loader.GetVertices();
    std::vector<Normal> normales = loader.GetNormals();
    // std::vector<TextureCoordinate> texCoords = loader.GetTextureCoordinates();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    Mesh * mesh = new Mesh(filename, vertices, indices, normales);

    mesh->CreateMaterial("phong");
    mesh->GetMaterial()->SetAmbientColor(material.ambient_color);
    mesh->GetMaterial()->SetDiffuseColor(material.diffuse_color);
    mesh->GetMaterial()->SetSpecularColor(material.specular_color);
    mesh->GetMaterial()->SetSpecularExponent(material.shininess);

    return mesh;
}

Mesh::~Mesh()
{
    m_Vertices.clear();
    m_Indices.clear();

    delete m_VAO;
    delete m_VBO_pos;
    delete m_IBO;
}

void Mesh::SetUp()
{
    m_VAO->Bind();
    m_VBO_pos->Bind();
    m_VBO_norm->Bind();
    m_IBO->Bind();
}

void Mesh::Clear()
{
    m_VAO->Unbind();
    m_VBO_pos->Unbind();
    m_VBO_norm->Unbind();
    m_IBO->Unbind();
}

void Mesh::SetName(std::string name)
{
    m_Name = name;
}

void Mesh::SetVertices(std::vector<Vertex> vertices)
{
    m_Vertices = vertices;
}

void Mesh::SetNormales(std::vector<Normal> normales)
{
    m_Normales = normales;
}

void Mesh::SetIndices(std::vector<IndexesFace> indices)
{
    m_Indices = indices;
}

void Mesh::CreateMaterial(std::string shaderFile)
{
    m_Material = new Material();
    m_Material->AddShader(shaderFile);
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
    return m_VBO_pos;
}

IndexBuffer *Mesh::GetIBO() const
{
    return m_IBO;
}

std::vector<Vertex> Mesh::GetVertices() const
{
    return m_Vertices;
}

std::vector<Normal> Mesh::GetNormales() const
{
    return m_Normales;
}

std::vector<IndexesFace> Mesh::GetIndices() const
{
    return m_Indices;
}

glm::mat4 Mesh::GetMatrix() const
{  
    return m_Matrix;
}

Material* Mesh::GetMaterial() const
{  
    return m_Material;
}

glm::vec3 Mesh::GetPosition() const
{
    return m_Position;
}

glm::vec3 Mesh::GetScale() const
{
    return m_Scale;
}

glm::vec3 Mesh::GetRotation() const
{
    return m_Rotation;
}

void Mesh::Translate(glm::vec3 translation)
{
    m_Position = translation;
    m_Matrix = glm::translate(m_Matrix, m_Position);
}

void Mesh::Rotate(GLfloat angle, glm::vec3 axis)
{
    m_Matrix = glm::rotate(m_Matrix, angle, axis);
}

void Mesh::Scale(glm::vec3 scale)
{
    m_Scale = scale;
    m_Matrix = glm::scale(m_Matrix, m_Scale);
}

glm::mat4 Mesh::ComputeMatrix()
{  
    glm::mat4 model(1.0f);

    glm::mat4 translation = glm::translate(model, m_Position);
    glm::mat4 rotation = glm::rotate(model, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)) *
                         glm::rotate(model, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
                         glm::rotate(model, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 scale = glm::scale(model, m_Scale);

    m_Matrix = translation * rotation * scale;
    return m_Matrix;
}