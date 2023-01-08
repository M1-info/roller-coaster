#include "ui/UIScene.h"

UIScene::UIScene(std::shared_ptr<Scene> scene, std::shared_ptr<Window> window, std::shared_ptr<FrameBuffer> fbo, std::shared_ptr<UIConsole> console)
    : m_Scene(scene), m_Window(window), m_FBO(fbo), m_UIConsole(console)
{
    m_UIFileController = new UIFilesController(m_UIConsole);
}

UIScene::~UIScene()
{
    delete m_UIFileController;
    m_UIFileController = nullptr;
}

void UIScene::SetSelectedMesh(std::shared_ptr<Mesh> mesh)
{
    // remove selection from previous mesh
    if (m_SelectedMesh != nullptr)
        m_SelectedMesh->m_IsSelected = false;

    // add selection to new mesh
    m_SelectedMesh = mesh;
    m_SelectedMesh->m_IsSelected = true;
}

void UIScene::SetUpDockSpace()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene dockspace", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleVar(3);

    ImGuiID scene_dockspace_id = ImGui::GetID("Scene Dockspace");
    ImGui::DockSpace(ImGui::GetID("Scene Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar);

    // dockspace window
    static bool first_dockspace = true;
    if (first_dockspace)
    {
        ImGui::DockBuilderRemoveNode(scene_dockspace_id);
        ImGui::DockBuilderAddNode(scene_dockspace_id);

        ImGuiID dock_main_id = scene_dockspace_id;
        ImGuiID dock_id_left, dock_id_right;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, &dock_id_left, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.8f, &dock_id_right, &dock_main_id);

        ImGui::DockBuilderDockWindow("Scene", dock_id_right);
        ImGui::DockBuilderDockWindow("Scene infos", dock_id_left);

        ImGui::DockBuilderFinish(scene_dockspace_id);
        first_dockspace = false;
    }

    ImGui::End();

    ImGui::Begin("Scene infos", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGuiID scene_graph_dockspace_id = ImGui::GetID("Scene Graph Dockspace");
    ImGui::DockSpace(ImGui::GetID("Scene Graph Dockspace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar);

    // dockspace window
    static bool first_graph = true;
    if (first_graph)
    {
        ImGui::DockBuilderRemoveNode(scene_graph_dockspace_id);
        ImGui::DockBuilderAddNode(scene_graph_dockspace_id);

        ImGuiID dock_main_id = scene_graph_dockspace_id;
        ImGuiID dock_id_bottom, dock_id_top;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.5f, &dock_id_bottom, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, &dock_id_top, &dock_main_id);

        ImGui::DockBuilderDockWindow("Scene graph", dock_id_top);
        ImGui::DockBuilderDockWindow("Selected mesh", dock_id_bottom);

        ImGui::DockBuilderFinish(scene_graph_dockspace_id);
        first_graph = false;
    }

    ImGui::End();
}

void UIScene::Render()
{
    SetUpDockSpace();
    SceneWindow();
    SceneGraphWindow();

    if (m_SelectedMesh != nullptr)
        SelectedMeshWindow();
}

void UIScene::SceneWindow()
{
    float aspectRatio = 16.0f / 9.0f;
    uint64_t textureID = m_FBO->GetColorBuffer();
    ImGui::Begin("Scene");
    ImGui::Image((ImTextureID)textureID,
                 ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowWidth() / aspectRatio),
                 ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();
}

void UIScene::SceneGraphWindow()
{
    ImGui::Begin("Scene graph", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(m_Fonts["Title"]);
    ImGui::Text("Scene graph");
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0, 2.0f));

    for (auto &mesh : m_Scene->GetObjects())
    {

        ImGui::PushID(mesh.get());

        if (mesh->GetChildren().size() > 0 || mesh->GetType() == MeshType::RAILS) // if mesh has children, display as tree
            SceneGraphElementTree(mesh);
        else // if mesh has no children, display as selectable
            SceneGraphElement(mesh);

        ImGui::PopID();
    }

    ImGui::End();
}

void UIScene::SceneGraphElement(std::shared_ptr<Mesh> mesh)
{
    ImGui::PushID(mesh.get());

    ImGui::Columns(2);

    if (mesh->GetParent() == nullptr)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 23.0f);
    else
        ImGui::SetCursorPosX(ImGui::GetCursorPosX());

    if (ImGui::Selectable(mesh->GetName().c_str(), m_SelectedMesh == mesh))
    {
        // remove selection if already selected and clicked
        if (m_SelectedMesh == mesh)
        {
            m_SelectedMesh->ToggleIsSelected();
            m_SelectedMesh.reset();
        }
        // add selection to new mesh if is not the same as the previous one
        else
        {
            if (m_SelectedMesh != nullptr)
                m_SelectedMesh->ToggleIsSelected();

            m_SelectedMesh = mesh;
            mesh->ToggleIsSelected();
        }
    }

    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Delete"))
    {
        m_SelectedMesh.reset();
        mesh->m_IsSelected = false;

        if (mesh->GetType() == MeshType::CONTROL_POINT)
        {
            std::shared_ptr<Rails> parent = std::dynamic_pointer_cast<Rails>(mesh->GetParent());
            parent->m_DrawRails = false;
            parent->RemoveChildren(mesh);
            parent->Update();
        }
        else
            m_Scene->Remove(mesh);
    }
    ImGui::PopStyleColor(3);

    ImGui::Columns(1);

    ImGui::PopID();
}

void UIScene::SceneGraphElementTree(std::shared_ptr<Mesh> mesh)
{

    if (m_SelectedMesh == mesh)
    {
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImU32 col = ImColor(ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered]);
        ImGui::RenderFrame(pos, ImVec2(pos.x + ImGui::GetContentRegionMax().x, pos.y + ImGui::GetTextLineHeight()), col, false);
    }

    if (ImGui::TreeNode(mesh->GetName().c_str()))
    {
        if (m_SelectedMesh == nullptr)
        {
            // if (m_SelectedMesh != nullptr)
            //     m_SelectedMesh->m_IsSelected = false;
            m_SelectedMesh = mesh;
            mesh->m_IsSelected = true;
        }

        if (mesh->GetType() == MeshType::RAILS)
        {
            std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(mesh);
            RailsWindow(rails);
            // for (auto &child : rails->GetRails())
            // {
            //     if (child->GetChildren().size() > 0)
            //         SceneGraphElementTree(child);
            //     else
            //         SceneGraphElement(child);

            //     if (child != m_SelectedMesh)
            //         child->m_IsSelected = false;
            // }
        }

        for (auto &child : mesh->GetChildren())
        {
            if (child->GetChildren().size() > 0)
                SceneGraphElementTree(child);
            else
                SceneGraphElement(child);

            if (child != m_SelectedMesh)
                child->m_IsSelected = false;
        }
        ImGui::TreePop();
    }
    else
    {
        if (m_SelectedMesh == mesh)
        {
            m_SelectedMesh.reset();
            mesh->m_IsSelected = false;
        }
    }
}

void UIScene::SelectedMeshWindow()
{
    ImGui::Begin("Selected mesh", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::Dummy(ImVec2(0, 2.0f));

    ImGui::SetNextWindowDockID(ImGui::GetID("Scene Graph Dockspace"), ImGuiCond_FirstUseEver);

    ImGui::PushFont(m_Fonts["Title"]);
    ImGui::Text(m_SelectedMesh->GetName().c_str());
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    SelectedMeshTransforms();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 5.0f));

    SelectedMeshMaterial();

    ImGui::End();
}

void UIScene::SelectedMeshTransforms()
{
    float x_spacing = 0.0f;
    float y_spacing = 2.0f;

    ImGui::PushFont(m_Fonts["Subtitle"]);
    ImGui::Text("Transform");
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    glm::vec3 *position;
    if (m_SelectedMesh->GetType() == MeshType::CONTROL_POINT)
        position = m_SelectedMesh->GetVertexPtr(0);
    else
        position = m_SelectedMesh->GetTransform()->GetPositionPtr();

    SelectedMeshTransformComponent("Position", &position->x, &position->y, &position->z);

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    glm::vec3 *rotation = m_SelectedMesh->GetTransform()->GetRotationPtr();
    SelectedMeshTransformComponent("Rotation", &rotation->x, &rotation->y, &rotation->z, 1.0f, 0.0f);

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    glm::vec3 *scale = m_SelectedMesh->GetTransform()->GetScalePtr();
    SelectedMeshTransformComponent("Scale", &scale->x, &scale->y, &scale->z, 0.1f, 1.0f);
}

void UIScene::SelectedMeshTransformComponent(std::string component, float *x, float *y, float *z, float step, float resetValue)
{

    ImGui::PushID(component.c_str());

    ImGui::Columns(2);

    ImGui::SetColumnWidth(0, ImGui::CalcTextSize(component.c_str()).x + 10);
    ImGui::Text(component.c_str());

    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

    const char *components[3] = {"X", "Y", "Z"};
    float *values[3] = {x, y, z};
    ImVec4 colors[3][2] = {
        // base color                    hovered color
        {ImVec4(0.6f, 0.0f, 0.0f, 1.0f), ImVec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {ImVec4(0.0f, 0.6f, 0.0f, 1.0f), ImVec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {ImVec4(0.0f, 0.0f, 0.6f, 1.0f), ImVec4(0.0f, 0.0f, 1.0f, 1.0f)}};

    // iterate through components
    for (int i = 0; i < 3; i++)
    {
        // reset button
        ImGui::PushStyleColor(ImGuiCol_Button, colors[i][0]);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors[i][1]);
        if (ImGui::Button(components[i]))
        {
            *values[i] = resetValue;
            m_SelectedMesh->Update();
            if (m_SelectedMesh->GetParent() != nullptr)
                m_SelectedMesh->GetParent()->Update();
        }
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        // drag float
        std::string label = "##" + std::string(components[i]);
        if (ImGui::DragFloat(label.c_str(), values[i], step))
        {
            m_SelectedMesh->Update();
            if (m_SelectedMesh->GetParent() != nullptr)
                m_SelectedMesh->GetParent()->Update();
        }

        ImGui::PopItemWidth();
        ImGui::SameLine();
    }

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
}

void UIScene::SelectedMeshMaterial()
{

    float x_spacing = 0.0f;
    float y_spacing = 2.0f;

    ImGui::PushFont(m_Fonts["Subtitle"]);
    ImGui::Text("Material");
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    // color
    ImGui::ColorEdit3("Color", (float *)m_SelectedMesh->GetMaterial()->GetMaterialColorPtr());

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    // ambient color
    ImGui::ColorEdit3("Ambient Color", (float *)m_SelectedMesh->GetMaterial()->GetAmbientColorPtr());

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    // Diffuse color
    ImGui::ColorEdit3("Diffuse Color", (float *)m_SelectedMesh->GetMaterial()->GetDiffuseColorPtr());

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    // specular color
    ImGui::ColorEdit3("Specular Color", (float *)m_SelectedMesh->GetMaterial()->GetSpecularColorPtr());
}

void UIScene::FilesControllerWindow(std::shared_ptr<Rails> rails)
{
    float x_spacing = 0.0f;
    float y_spacing = 2.0f;

    m_UIFileController->SelectFileWindow();

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    if (ImGui::Button("Unselect file"))
        m_UIFileController->SetSelectedFile("");

    ImGui::SameLine();

    m_UIFileController->LoadFileWindow(rails);

    ImGui::SameLine();

    m_UIFileController->DeleteFileWindow();

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));

    m_UIFileController->SaveFileWindow(rails);

    ImGui::Dummy(ImVec2(x_spacing, y_spacing));
}

void UIScene::RailsWindow(std::shared_ptr<Rails> rails)
{
    FilesControllerWindow(rails);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Add control point"))
    {
        glm::vec3 lastPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        if (rails->GetChildren().size() > 0)
            lastPosition = *rails->GetChildren().back()->GetVertexPtr(0);

        rails->AddChildren(std::make_shared<ControlPoint>(lastPosition, rails->GetChildren().size()));
        rails->UpdateControlPoints();

        if (m_SelectedMesh != nullptr)
            m_SelectedMesh->ToggleIsSelected();

        m_SelectedMesh = rails->GetChildren().back();
        m_SelectedMesh->ToggleIsSelected();
        m_UIConsole->AddLog("Control point added. You can set his position in the appropriate window", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    ImGui::PopStyleColor(2);
    ImGui::Dummy(ImVec2(0.0f, 0.2f));
}