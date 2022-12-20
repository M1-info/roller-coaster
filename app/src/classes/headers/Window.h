#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>
#include <memory>

#include "Camera.h"

class Window
{
public:
    Window() = default;
    Window(float width, float height);
    ~Window();

    void Init();
    void Destroy() const;
    static void OnResize(GLFWwindow* window, int width, int height);

    inline GLFWwindow* GetWindow() const { return m_Window; }
    inline std::shared_ptr<Camera> GetCamera() const { return m_Camera; }
    inline float GetWidth() const { return m_Width; }
    inline float GetHeight() const { return m_Height; }

    inline void SetCamera(std::shared_ptr<Camera> camera) { m_Camera = camera; }

private:
    GLFWwindow* m_Window;
    float m_Width, m_Height;
    std::shared_ptr<Camera> m_Camera;
};

#endif // WINDOW_H