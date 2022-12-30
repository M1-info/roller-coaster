#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include "FrameBuffer.h"
#include "Window.h"
#include "Scene.h"
#include "Mesh.h"
#include "ControlPoint.h"
#include "Light.h"
#include "Rails.h"
#include "Cart.h"

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
    void SceneGraph();
    void MeshInfo();
    void MeshTransform(std::string component, glm::vec3 *value, float step = 0.1f, glm::vec3 resetValue = glm::vec3(0.0f));
    void CameraInfo();
    void SwapCameraPosition();
    void ConsoleLog();
    void SceneRender();

    inline void SetWindow(Window *window) { m_Window = window; }
    inline void SetScene(Scene *scene) { m_Scene = scene; }
    void SetSelectedMesh(std::shared_ptr<Mesh> mesh);
    inline void SetLight(Light *light) { m_Light = light; }
    void AddLog(const std::string log, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    inline void SetFBO(FrameBuffer *fbo) { m_FBO = fbo; }

    inline bool GetIsAnimating() const { return m_IsAnimating; }

private:
    Window *m_Window;
    Scene *m_Scene;
    Light *m_Light;
    std::shared_ptr<Mesh> m_SelectedMesh;
    std::vector<std::pair<std::string, ImVec4>> m_Logs;
    FrameBuffer *m_FBO;
    float m_AspectRatio = 16.0f / 9.0f;
    bool m_IsAnimating = false;
};

#endif // UI_H