#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <string>
#include <iostream>
#include <memory>
#include <algorithm>

#include "Camera.h"
#include "buffers/FrameBuffer.h"

struct PixelInfo
{
    float ObjectID;
    float DrawID;
    float PrimID;

    PixelInfo()
    {
        ObjectID = 0.0f;
        DrawID = 0.0f;
        PrimID = 0.0f;
    }
};

class Window
{
public:
    Window() = default;
    Window(float width, float height);
    ~Window();

    void Init();
    void Destroy() const;

    // getters
    inline GLFWwindow *GetWindow() const { return m_Window; }
    inline float GetWidth() const { return m_Width; }
    inline float GetHeight() const { return m_Height; }
    inline float GetAspectRatio() const { return m_Width / m_Height; }

    inline std::shared_ptr<Camera> GetCamera() const { return m_Camera; }

    inline GLint GetFBO() const { return m_FBO->GetFrameBuffer(); }

    inline float GetIsMousePressed() const { return m_IsMousePressed; }

    // setters
    inline void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; }
    inline void SetFBO(std::shared_ptr<FrameBuffer> fbo) { m_FBO = fbo; }

    // window methods
    void PollEvents() const;
    void SwapBuffers() const;
    void Clear() const;

    // events callbacks
    static void OnResize(GLFWwindow *window, int width, int height);
    static void OnMouseMove(GLFWwindow *window, double xpos, double ypos);
    static void OnMouseClick(GLFWwindow *window, int button, int action, int mods);
    static void OnKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods);

private:
    GLFWwindow *m_Window;
    float m_Width, m_Height;
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<FrameBuffer> m_FBO;

    bool m_IsMousePressed;
};

#endif // WINDOW_H