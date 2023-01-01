#include "headers/Rail.h"

Rail::Rail(const std::string filename, int index)
{

    m_VBO_norm = nullptr;
    m_IBO = nullptr;

    m_Type = MeshType::RAIL;

    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_Rotation = glm::vec3(0.0f);
    m_Matrix = glm::mat4(1.0f);

    OBJLoader loader(filename);

    std::vector<glm::vec3> vertices = loader.GetVertices();
    std::vector<glm::vec3> normales = loader.GetNormals();
    std::vector<IndexesFace> indices = loader.GetFaces();
    OBJMaterial material = loader.GetMaterials()[0];

    m_Name = filename.substr(0, filename.find_last_of('.'));
    m_Name[0] = toupper(m_Name[0]);
    m_Name += std::to_string(index);

    CreateMaterial("phong");
    m_Material->SetMaterialColor(Color(1.0f, 0.0f, 0.0f));
    m_Material->SetAmbientColor(material.ambient_color);
    m_Material->SetDiffuseColor(material.diffuse_color);
    m_Material->SetSpecularColor(material.specular_color);
    m_Material->SetSpecularExponent(material.shininess);

    SetUpShader();

    for (auto i : indices)
    {
        m_Vertices.push_back(vertices[i.vertices[0]]);
        m_Normales.push_back(normales[i.normals[0]]);
        m_Vertices.push_back(vertices[i.vertices[1]]);
        m_Normales.push_back(normales[i.normals[1]]);
        m_Vertices.push_back(vertices[i.vertices[2]]);
        m_Normales.push_back(normales[i.normals[2]]);
    }

    m_Vertices_size = m_Vertices.size() * sizeof(glm::vec3);
    m_Normales_size = m_Normales.size() * sizeof(glm::vec3);
    m_Indices_size = m_Indices.size() * sizeof(unsigned int);

    // create vertex array
    m_VAO = new VertexArray();

    // create vertex buffer for positions
    m_VBO_pos = new VertexBuffer(m_Vertices.data(), m_Vertices_size);
    VertexBufferLayout layout_pos;
    layout_pos.Push<float>(3); // position
    m_VAO->AddBuffer(*m_VBO_pos, layout_pos);

    // create vertex buffer for normales
    m_VBO_norm = new VertexBuffer(m_Normales.data(), m_Normales_size);
    VertexBufferLayout layout_norm;
    layout_norm.Push<float>(3); // normales
    m_VAO->AddBuffer(*m_VBO_norm, layout_norm, 1);

    Clear();
}

void Rail::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    m_Material->GetShader()->Unbind();
    m_VAO->Unbind();
}

void Rail::SetUpShader()
{
    Shader *shader = m_Material->GetShader();

    Color materialAmbient = m_Material->GetAmbientColor();
    Color materialDiffuse = m_Material->GetDiffuseColor();
    Color materialSpecular = m_Material->GetSpecularColor();
    Color materialColor = m_Material->GetMaterialColor();
    float specularExponent = m_Material->GetSpecularExponent();

    shader->Bind();

    shader->SetUniform3f("u_material.color", materialColor.r, materialColor.g, materialColor.b);
    shader->SetUniform3f("u_material.coeffAmbient", materialAmbient.r, materialAmbient.g, materialAmbient.b);
    shader->SetUniform3f("u_material.coeffDiffuse", materialDiffuse.r, materialDiffuse.g, materialDiffuse.b);
    shader->SetUniform3f("u_material.coeffSpecular", materialSpecular.r, materialSpecular.g, materialSpecular.b);
    shader->SetUniform1f("u_material.specularExponent", specularExponent);
    shader->SetUniform1i("u_isSelected", 0);

    shader->Unbind();
}