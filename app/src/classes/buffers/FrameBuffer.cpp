#include "buffers/FrameBuffer.h"


FrameBuffer::FrameBuffer(float width, float height) : m_Width(width), m_Height(height)
{
    Init();
}

FrameBuffer::~FrameBuffer()
{
    Destroy();
}

void FrameBuffer::Init()
{
    // Create FrameBuffer
    glCreateFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    // Create ColorBuffer
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

    // Create DepthBuffer
    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_DepthBuffer);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Destroy()
{
    glDeleteFramebuffers(1, &m_FrameBuffer);
    glDeleteTextures(1, &m_ColorBuffer);
    glDeleteRenderbuffers(1, &m_DepthBuffer);
}

void FrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void FrameBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}