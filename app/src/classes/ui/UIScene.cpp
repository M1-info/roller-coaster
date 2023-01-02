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

void UIScene::SceneWindow()
{
    float aspectRatio = m_Window->GetAspectRatio();
    uint64_t textureID = m_FBO->GetColorBuffer();
    ImGui::Begin("Scene", nullptr);
    ImGui::Image((ImTextureID)textureID,
                 ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowWidth() / aspectRatio),
                 ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();
}

void UIScene::SceneGraphWindow()
{
    ImGui::Begin("Scene Graph");

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

    if (ImGui::Selectable(mesh->GetName().c_str(), m_SelectedMesh == mesh))
    {
        // remove selection if already selected and clicked
        if (m_SelectedMesh == mesh)
        {
            m_SelectedMesh.reset();
            mesh->m_IsSelected = false;
        }
        // add selection to new mesh if is not the same as the previous one
        else
        {
            m_SelectedMesh = mesh;
            mesh->m_IsSelected = true;
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
    if (ImGui::TreeNode(mesh->GetName().c_str()))
    {
        if (mesh->GetType() == MeshType::RAILS)
        {
            std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(mesh);
            RailsWindow(rails);
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
}

void UIScene::SelectedMeshWindow()
{
    ImGui::Begin("Selected Mesh");

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
            m_SelectedMesh->GetTransform()->SetIsDirty(true);
        }
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        // drag float
        std::string label = "##" + std::string(components[i]);
        if (ImGui::DragFloat(label.c_str(), values[i], step))
        {
            m_SelectedMesh->GetTransform()->SetIsDirty(true);
            m_SelectedMesh->Update();
            if (m_SelectedMesh->GetParent() != nullptr)
            {
                m_SelectedMesh->GetParent()->GetTransform()->SetIsDirty(true);
                m_SelectedMesh->GetParent()->Update();
            }
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
        rails->Update();
        m_UIConsole->AddLog("Control point added. You can set his position in the appropriate window", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    }
    ImGui::PopStyleColor(2);
    ImGui::Dummy(ImVec2(0.0f, 0.2f));
}