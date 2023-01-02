#include "utils/CubeTexture.h"

CubeTexture::CubeTexture(std::vector<std::string> files)
: m_TextureID(0), m_Files(files), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BitsPerPixel(0)
{
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

    for (unsigned int i = 0; i < m_Files.size(); i++)
    {
        m_LocalBuffer = stbi_load(m_Files[i].c_str(), &m_Width, &m_Height, &m_BitsPerPixel, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
        stbi_image_free(m_LocalBuffer);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    Unbind();
}

CubeTexture::~CubeTexture()
{
}

void CubeTexture::Bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);
}

void CubeTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}