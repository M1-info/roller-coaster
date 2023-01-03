#pragma once
#ifndef UISCENE_H
#define UISCENE_H

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "UIConsole.h"
#include "UIFilesController.h"

#include "Window.h"
#include "Scene.h"
#include "buffers/FrameBuffer.h"

class UIScene
{

public:
    UIScene(std::shared_ptr<Scene> scene, std::shared_ptr<Window> window, std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<UIConsole> console);
    ~UIScene();

    void SetSelectedMesh(std::shared_ptr<Mesh> mesh);
    inline std::shared_ptr<Mesh> GetSelectedMesh() const { return m_SelectedMesh; }
    inline void SetFonts(std::map<std::string, ImFont *> fonts) { m_Fonts = fonts; }

    void SetUpDockSpace();
    void Render();

    void SceneWindow();

    void SceneGraphWindow();
    void SceneGraphElement(std::shared_ptr<Mesh> mesh);
    void SceneGraphElementTree(std::shared_ptr<Mesh> mesh);

    void SelectedMeshWindow();
    void SelectedMeshTransforms();
    void SelectedMeshTransformComponent(std::string component, float *x, float *y, float *z, float step = 0.1f, float resetValue = 0.0f);
    void SelectedMeshMaterial();

    void FilesControllerWindow(std::shared_ptr<Rails> rails);
    void RailsWindow(std::shared_ptr<Rails> rails);

private:
    std::shared_ptr<UIConsole> m_UIConsole;
    UIFilesController *m_UIFileController;

    std::shared_ptr<FrameBuffer> m_FBO;
    std::shared_ptr<Scene> m_Scene;
    std::shared_ptr<Window> m_Window;
    std::shared_ptr<Mesh> m_SelectedMesh;

    std::map<std::string, ImFont *> m_Fonts;
};

#endif // UISCENE_H