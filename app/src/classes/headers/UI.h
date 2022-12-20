#pragma once
#ifndef UI_H
#define UI_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Window.h"
#include "Scene.h"

class UI
{
public:
    UI() = default;
    UI(Window* window, Scene* scene);
    ~UI();

    void Init();
    void Destroy() const;
    void Render() const;

    void FrameRate(ImGuiViewport* viewport) const;
    void SceneInfo(ImGuiViewport* viewport) const;
    // void MeshInfo() const;
    // void CameraInfo() const;

    inline void SetWindow(Window* window) { m_Window = window; }
    inline void SetScene(Scene* scene) { m_Scene = scene; }

private:
    Window* m_Window;
    Scene* m_Scene;

};

#endif // UI_H