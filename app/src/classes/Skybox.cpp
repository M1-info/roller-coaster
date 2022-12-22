#include "headers/Skybox.h"

Skybox::Skybox() : m_VAO(nullptr), m_VBO(nullptr), m_Texture(nullptr), m_Shader(nullptr)
{
}

Skybox::~Skybox()
{
}

void Skybox::Init()
{

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
    m_VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VBO, layout);
    m_Shader = new Shader("skybox");

    SetupCubeMap();

    m_Shader->Bind();
    m_Shader->SetUniform1i("u_texture", 0);
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

void Skybox::Draw(glm::mat4 projection, glm::mat4 view)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    m_Shader->Bind();
    m_VAO->Bind();
    glActiveTexture(GL_TEXTURE0);
    m_Texture->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_Shader->Unbind();
    m_VAO->Unbind();
    m_Texture->Unbind();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
}
