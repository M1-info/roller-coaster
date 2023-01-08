#include "meshes/Skybox.h"

Skybox::Skybox()
{
    m_VAO = nullptr;
    m_VBO_positions = nullptr;
    m_IBO = nullptr;
    m_Parent = nullptr;
    m_Texture = nullptr;
    m_Material = nullptr;
}

void Skybox::Init()
{

    m_Type = MeshType::SKYBOX;

    // skybox vertices
    std::vector<float> vertices = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    m_VAO = new VertexArray();
    m_VBO_positions = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO_positions, layout);

    CreateMaterial("skybox");

    SetupCubeMap();

    m_Material->GetShader()->Bind();
    m_Material->GetShader()->SetUniform1i("u_texture", 0);
    m_Material->GetShader()->Unbind();
}

void Skybox::SetupCubeMap()
{
    std::string filepath;
#if VISUAL_STUDIO
    filepath = "src/assets/images/skybox/";
#elif MINGW
    filepath = "app\\src\\assets\\images\\skybox\\";
#endif

    std::vector<std::string> faces = {
        filepath + "right.jpg",
        filepath + "left.jpg",
        filepath + "top.jpg",
        filepath + "bottom.jpg",
        filepath + "back.jpg",
        filepath + "front.jpg",
    };

    m_Texture = new CubeTexture(faces);
}

void Skybox::Draw()
{
    m_Material->GetShader()->Bind();
    m_VAO->Bind();
    glActiveTexture(GL_TEXTURE0);
    m_Texture->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_Material->GetShader()->Bind();
    m_VAO->Unbind();
    m_Texture->Unbind();
}
