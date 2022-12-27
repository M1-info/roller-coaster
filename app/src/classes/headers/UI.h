#pragma once
#ifndef UI_H
#define UI_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "FrameBuffer.h"
#include "Window.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"

class UI
{
public:
    UI() = default;
    UI(Window *window, Scene *scene);
    ~UI();

    void Init();
    void Destroy();
    void Render();

    void FrameRate(ImGuiViewport *viewport) const;
    void SceneInfo();
    void SceneGraph();
    void MeshInfo();
    void MeshTransform(std::string component, glm::vec3 &value, float resetValue = 0.0f);
    void CameraInfo();
    void ConsoleLog();


    inline void SetWindow(Window *window) { m_Window = window; }
    inline void SetScene(Scene *scene) { m_Scene = scene; }
    inline void SetSelectedMesh(std::shared_ptr<Mesh> mesh) { m_SelectedMesh = mesh; }
    inline void SetLight(Light * light) { m_Light = light; }
    inline void AddLog(const std::string log) { m_Logs.push_back(log); }

private:
    Window *m_Window;
    Scene *m_Scene;
    Light *m_Light;
    std::shared_ptr<Mesh> m_SelectedMesh;
    std::vector<std::string> m_Logs;
};

#endif // UI_H