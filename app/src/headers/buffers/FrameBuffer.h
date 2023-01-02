#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>

#include <iostream>

class FrameBuffer
{
public:
    FrameBuffer() = default;
    FrameBuffer(float width, float height);
    ~FrameBuffer();

    void Init();
    void Destroy();
    void Bind();
    void Unbind();

    inline unsigned int GetColorBuffer() const { return m_ColorBuffer; }
    inline unsigned int GetDepthBuffer() const { return m_DepthBuffer; }
    inline unsigned int GetFrameBuffer() const { return m_FrameBuffer; }

    inline float GetWidth() { return m_Width; }
    inline float GetHeight() { return m_Height; }

    inline void Resize(float width, float height)
    {
        m_Width = width;
        m_Height = height;
        Destroy();
        Init();
    }

private:
    float m_Width;
    float m_Height;
    unsigned int m_ColorBuffer;
    unsigned int m_DepthBuffer;
    unsigned int m_FrameBuffer;
};


#endif // FRAMEBUFFER_H