#include "utils/Material.h"

Material::Material()
    : m_Shininess(0.0f), m_AmbientColor(Color()),
      m_DiffuseColor(Color()), m_SpecularColor(Color()),
      m_EmissiveColor(Color()), m_Shader(nullptr), m_MaterialColor(Color())
{
}

Material::Material(float shininess, Color ambientColor, Color diffuseColor, Color specularColor, Color emissiveColor)
    : m_Shininess(shininess), m_AmbientColor(ambientColor),
      m_DiffuseColor(diffuseColor), m_SpecularColor(specularColor),
      m_EmissiveColor(emissiveColor), m_Shader(nullptr), m_MaterialColor(Color())
{
}

Material::~Material()
{
    delete m_Shader;
}

Shader *Material::GetShader() const
{
    return m_Shader;
}

void Material::SetShader(Shader *shader)
{
    m_Shader = shader;
}

void Material::AddShader(std::string filename)
{
    m_Shader = new Shader(filename);
}

void Material::SetUpShader(std::shared_ptr<Light> light)
{
    m_Shader->Bind();

    m_Shader->SetUniform3f("u_material.color", m_MaterialColor.r, m_MaterialColor.g, m_MaterialColor.b);
    m_Shader->SetUniform3f("u_material.ambient", m_AmbientColor.r, m_AmbientColor.g, m_AmbientColor.b);
    m_Shader->SetUniform3f("u_material.diffuse", m_DiffuseColor.r, m_DiffuseColor.g, m_DiffuseColor.b);
    m_Shader->SetUniform3f("u_material.specular", m_SpecularColor.r, m_SpecularColor.g, m_SpecularColor.b);
    m_Shader->SetUniform1f("u_material.shininess", m_Shininess);

    glm::vec3 lightPosition = light->GetTransform()->GetPosition();
    glm::vec3 lightColor = light->GetColor();
    float lightIntensity = light->GetIntensity();

    m_Shader->SetUniform1f("u_light.intensity", lightIntensity);
    m_Shader->SetUniform3f("u_light.position", lightPosition.x, lightPosition.y, lightPosition.z);
    m_Shader->SetUniform3f("u_light.color", lightColor.x, lightColor.y, lightColor.z);

    m_Shader->Unbind();
}

void Material::UpdateShader(glm::mat4 projectionView, glm::mat4 model, glm::mat4 view, std::shared_ptr<Light> light, glm::vec3 cameraPosition)
{
    m_Shader->Bind();

    // some model don't use phong lighting
    // so we don't pass the light to the shader
    if (light != nullptr)
    {
        glm::vec3 lightPosition = light->GetTransform()->GetPosition();
        glm::vec3 lightColor = light->GetColor();
        float lightIntensity = light->GetIntensity();

        m_Shader->SetUniform1f("u_light.intensity", lightIntensity);
        m_Shader->SetUniform3f("u_light.position", lightPosition.x, lightPosition.y, lightPosition.z);
        m_Shader->SetUniform3f("u_light.color", lightColor.x, lightColor.y, lightColor.z);

        m_Shader->SetUniform3f("u_cameraPosition", cameraPosition.x, cameraPosition.y, cameraPosition.z);

        m_Shader->SetUniformMat4f("u_view", view);

        m_Shader->SetUniform3f("u_material.color", m_MaterialColor.r, m_MaterialColor.g, m_MaterialColor.b);
        m_Shader->SetUniform3f("u_material.ambient", m_AmbientColor.r, m_AmbientColor.g, m_AmbientColor.b);
        m_Shader->SetUniform3f("u_material.diffuse", m_DiffuseColor.r, m_DiffuseColor.g, m_DiffuseColor.b);
        m_Shader->SetUniform3f("u_material.specular", m_SpecularColor.r, m_SpecularColor.g, m_SpecularColor.b);
        m_Shader->SetUniform1f("u_material.shininess", m_Shininess);
    }

    // some model like the skybox have no model matrix
    // so we pass just the identity matrix
    if (model != glm::mat4(0.0))
    {
        m_Shader->SetUniformMat4f("u_model", model);
    }

    m_Shader->SetUniformMat4f("u_projectionView", projectionView);

    m_Shader->Unbind();
}
