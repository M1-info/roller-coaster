#pragma once
#ifndef UI_H
#define UI_H

#include <iostream>
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_internal.h>

#include "UIScene.h"
#include "UiConsole.h"
#include "UIFilesController.h"

#include "buffers/FrameBuffer.h"

#include "Window.h"
#include "Scene.h"
#include "Light.h"

#include "meshes/Mesh.h"
#include "meshes/Rails.h"
#include "meshes/Cart.h"
#include "meshes/ControlPoint.h"

class UI
{
public:
    UI(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene, std::shared_ptr<Light> light, std::shared_ptr<FrameBuffer> fbo);
    ~UI();

    inline void SetIsAnimating(bool isAnimating) { m_IsAnimating = isAnimating; }
    inline bool GetIsAnimating() const { return m_IsAnimating; }

    void Init();
    void Destroy();
    void Render();

    void SetUpDockSpace();

    void RenderWindow();
    void CameraWindow();
    void LightWindow();

private:
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Light> m_Light;
    std::shared_ptr<Scene> m_Scene;

    UIScene *m_UIScene;
    std::shared_ptr<UIConsole> m_UIConsole;

    std::map<std::string, ImFont *> m_Fonts;

    bool m_IsAnimating = false;
    const char * m_CurveSelected = "Bezier";
};

#endif // UI_H