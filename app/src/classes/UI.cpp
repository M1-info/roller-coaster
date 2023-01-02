#include "headers/UI.h"

UI::UI(Window *window, Scene *scene) : m_Window(window), m_Scene(scene)
{
}

UI::~UI()
{
}

void UI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // enable docking
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void UI::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // full screen window only for docking
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Begin Dockspace
    ImGui::Begin("DockSpace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool first = true;
    if (first)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id);

        auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
        auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.15f, nullptr, &dockspace_id);
        auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.8f, nullptr, &dockspace_id);
        auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);

        ImGui::DockBuilderDockWindow("Scene", dock_id_right);
        ImGui::DockBuilderDockWindow("Scene Graph", dock_id_left);
        ImGui::DockBuilderDockWindow("Camera info", dock_id_left);
        ImGui::DockBuilderDockWindow("Selected mesh info", dock_id_left);
        ImGui::DockBuilderDockWindow("Console", dock_id_down);

        ImGui::DockBuilderFinish(dockspace_id);
        first = false;
        AddLog("Docking system initialized");
    }

    ImGui::End();

    /* scene infos window */
    SceneRender();
    SceneGraph();
    MeshInfo();
    CameraInfo();
    ConsoleLog();

    ImGui::Begin("Light");
    MeshTransform("Light", &m_Light->m_Position);
    ImGui::End();

    ImGui::Begin("Animation");
    if (ImGui::Checkbox("Animation", &m_IsAnimating))
    {
        if (m_IsAnimating)
        {
            std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_Scene->GetObjectByName("Rails"));
            std::shared_ptr<Cart> cart = std::dynamic_pointer_cast<Cart>(m_Scene->GetObjectByName("Chariot"));
            cart->SetRailsVertices(rails->GetVertices());
            cart->SetRailsTangents(rails->GetVerticesTangents());
        }
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::FrameRate(ImGuiViewport *viewport) const
{

    ImGui::Begin("Frame Rate", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
    ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
}

void UI::SceneGraph()
{
    ImGui::Begin("Scene Graph");
    for (auto mesh : m_Scene->GetObjects())
    {
        ImGui::PushID(mesh->GetName().c_str());

        ImGui::Dummy(ImVec2(0.0f, 3.0f));

        if (mesh->GetType() == MeshType::RAILS)
        {
            if (ImGui::TreeNode(mesh->GetName().c_str()))
            {
                if (mesh->GetType() == MeshType::RAILS)
                {
                    std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(mesh);
                    ImGui::Dummy(ImVec2(5.0f, 5.0f));
                    if (ImGui::Checkbox("Draw rails", &rails->m_DrawRails))
                        if (rails->m_DrawRails)
                        {
                            if (rails->GetChildren().empty())
                            {
                                AddLog("No control points, draw not possible", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                                rails->m_DrawRails = false;
                            }
                            else
                            {
                                rails->UpdateRails();
                                AddLog("Rails drawn", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                            }
                        }

                    ImGui::Dummy(ImVec2(0.0f, 3.0f));

                    // Select field
                    ImGui::Text("Select control points file");
                    ImGui::ListBoxHeader("", ImVec2(0, 50));

                    for (auto &file : rails->m_ControlPointsFiles)
                    {
                        bool isSelected = (file == rails->m_ControlPointsFileSelected);
                        if (ImGui::Selectable(file.c_str(), isSelected))
                        {
                            rails->m_ControlPointsFileSelected = file;
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::ListBoxFooter();

                    ImGui::Columns(2);
                    if (ImGui::Button("Load file"))
                    {
                        if (rails->m_ControlPointsFileSelected == "")
                        {
                            AddLog("No file selected", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        }
                        else
                        {
                            rails->LoadRails(rails->m_ControlPointsFileSelected.c_str());
                            rails->m_ControlPointsFileSelected = "";
                            AddLog("File loaded", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                        }
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Delete file"))
                    {
                        if (rails->m_ControlPointsFileSelected == "")
                        {
                            AddLog("No file selected", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                        }
                        else
                        {
                            // fro the moment we can delete file where rails were loaded from
                            bool succed = rails->DeleteFileRails(rails->m_ControlPointsFileSelected.c_str());
                            if (succed)
                            {
                                rails->m_ControlPointsFileSelected = "";
                                AddLog("File deleted", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                                rails->LoadControlPointsFiles();
                            }
                            else
                            {
                                AddLog("File not deleted", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                            }
                        }
                    }
                    ImGui::Columns(1);

                    ImGui::Dummy(ImVec2(0.0f, 5.0f));
                    ImGui::Columns(2);
                    if (ImGui::Button("Add control point"))
                    {
                        mesh->AddChildren(std::make_shared<ControlPoint>(glm::vec3(0.0f), mesh->GetChildren().size()));
                        mesh->Update();
                        AddLog("Control point added. You can set his position in the appropriate window", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    }
                    ImGui::NextColumn();
                    if (ImGui::Button("Save current points"))
                    {
                        bool isExported = rails->ExportRails();
                        std::string message;
                        if (isExported)
                            message = "Rails exported to " + std::string(rails->m_ControlPointsFileName);
                        else
                            message = "Error exporting rails to " + std::string(rails->m_ControlPointsFileName);

                        ImVec4 color = isExported ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                        AddLog(message, color);
                        rails->LoadControlPointsFiles();
                        rails->m_ControlPointsFileName = "controlPoints";
                    }
                    ImGui::Columns(1);
                }

                ImGui::Dummy(ImVec2(0.0f, 3.0f));

                for (std::shared_ptr<Mesh> child : mesh->GetChildren())
                {
                    ImGui::PushID(child->GetName().c_str());
                    ImGui::Columns(2);
                    if (ImGui::Selectable(child->GetName().c_str(), m_SelectedMesh == child))
                    {
                        if (m_SelectedMesh == child)
                        {
                            m_SelectedMesh.reset();
                            child->m_IsSelected = false;
                        }
                        else
                        {
                            m_SelectedMesh = child;
                            child->m_IsSelected = true;
                        }
                    }
                    ImGui::NextColumn();

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    if (ImGui::Button("Delete"))
                    {
                        if (child->GetParent()->GetType() == MeshType::RAILS)
                            std::dynamic_pointer_cast<Rails>(child->GetParent())->m_DrawRails = false;

                        child->m_IsSelected = false;
                        child->GetParent().reset();
                        mesh->RemoveChildren(child);
                        mesh->Update();
                        m_SelectedMesh.reset();
                        AddLog("Control point removed", ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    }

                    ImGui::PopStyleColor(3);
                    ImGui::Columns(1);
                    ImGui::PopID();

                    if (child != m_SelectedMesh)
                        child->m_IsSelected = false;
                }

                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::Columns(2);
            if (ImGui::Selectable(mesh->GetName().c_str(), m_SelectedMesh == mesh))
            {
                if (m_SelectedMesh == mesh)
                {
                    m_SelectedMesh.reset();
                    mesh->m_IsSelected = false;
                }
                else
                {
                    m_SelectedMesh = mesh;
                    mesh->m_IsSelected = true;
                }
            }

            ImGui::NextColumn();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            if (ImGui::Button("Delete"))
            {
                m_SelectedMesh.reset();
                mesh->m_IsSelected = false;
                m_Scene->Remove(mesh);
            }
            ImGui::PopStyleColor(3);
            ImGui::Columns(1);
        }
        ImGui::PopID();

        ImGui::Separator();
    }
    ImGui::End();
}

void UI::MeshInfo()
{
    ImGui::Begin("Selected mesh info");
    if (m_SelectedMesh == nullptr)
        ImGui::Text("No mesh selected");
    else
    {
        ImGui::Text(m_SelectedMesh->GetName().c_str());
        ImGui::Separator();
        ImGui::Text("Transform");

        ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Add some space between the two lines

        if (m_SelectedMesh->GetType() == MeshType::CONTROL_POINT)
        {
            glm::vec3 *vertex = m_SelectedMesh->GetVertexPtr(0);
            MeshTransform("Position", vertex);
        }
        else
            MeshTransform("Position", &m_SelectedMesh->m_Position);
        ImGui::Separator();
        MeshTransform("Rotation", &m_SelectedMesh->m_Rotation, 1.0f);
        ImGui::Separator();
        if (m_SelectedMesh->GetType() == MeshType::GROUND)
            MeshTransform("Scale", &m_SelectedMesh->m_Scale, 0.1f, glm::vec3(50.0f));
        else
            MeshTransform("Scale", &m_SelectedMesh->m_Scale, 0.1f, glm::vec3(1.0f));
    }
    ImGui::End();
}

void UI::MeshTransform(std::string component, glm::vec3 *value, float step, glm::vec3 resetValue)
{

    ImGui::PushID(component.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 100);
    ImGui::Text(component.c_str());
    ImGui::NextColumn();
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("X"))
        value->x = resetValue.x;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if (ImGui::DragFloat("##X", &value->x, step))
    {
        m_SelectedMesh->Update();
        if (m_SelectedMesh->GetParent() != nullptr)
        {
            m_SelectedMesh->GetParent()->Update();
            if (m_SelectedMesh->GetType() == MeshType::CONTROL_POINT)
            {
                std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_SelectedMesh->GetParent());
                if (rails->m_DrawRails)
                    rails->m_DrawRails = false;
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("Y"))
        value->y = resetValue.y;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if (ImGui::DragFloat("##Y", &value->y, step))
    {
        m_SelectedMesh->Update();
        if (m_SelectedMesh->GetParent() != nullptr)
        {
            m_SelectedMesh->GetParent()->Update();
            if (m_SelectedMesh->GetType() == MeshType::CONTROL_POINT)
            {
                std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_SelectedMesh->GetParent());
                if (rails->m_DrawRails)
                    rails->m_DrawRails = false;
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    if (ImGui::Button("Z"))
        value->z = resetValue.z;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if (ImGui::DragFloat("##Z", &value->z, step))
    {
        m_SelectedMesh->Update();
        if (m_SelectedMesh->GetParent() != nullptr)
        {
            m_SelectedMesh->GetParent()->Update();
            if (m_SelectedMesh->GetType() == MeshType::CONTROL_POINT)
            {
                std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_SelectedMesh->GetParent());
                if (rails->m_DrawRails)
                    rails->m_DrawRails = false;
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();
}

void UI::CameraInfo()
{
    std::shared_ptr<Camera> camera = m_Window->GetCamera();
    glm::vec3 *position = camera->GetPositionPtr();
    float *cameraSpeed = camera->GetMoveSpeedPtr();
    float *sensitivity = camera->GetMoveSensitivityPtr();

    float resetValue = 0.0f;

    ImGui::PushID("Camera");

    ImGui::Begin("Camera info");

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 80);
    ImGui::Text("Move speed");
    ImGui::NextColumn();
    ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 2));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Speed"))
        *cameraSpeed = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Speed", cameraSpeed, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("Sensitivity"))
        *sensitivity = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Sensitivity", sensitivity, 0.1f, 0.1f, 1.0f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::NextColumn();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::Text("Position");
    ImGui::NextColumn();

    ImGui::Dummy(ImVec2(0.0f, 20.0f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("X"))
        position->x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##X", &position->x, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if (ImGui::Button("Y"))
        position->y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Y", &position->y, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    if (ImGui::Button("Z"))
        position->z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Z", &position->z, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    SwapCameraPosition();

    ImGui::End();

    ImGui::PopID();
}

void UI::SwapCameraPosition()
{
    std::shared_ptr<Camera> camera = m_Window->GetCamera();
    std::shared_ptr<Mesh> cart = m_Scene->GetObjectByName("Chariot");

    if (ImGui::Checkbox("Swap camera position", &camera->m_IsOnCart))
    {
        if (camera->m_IsOnCart)
        {
            camera->SetPosition(cart->m_Position + glm::vec3(0.0f, 2.0f, 0.0f));
            AddLog("Camera position is now on the chariot", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else
        {
            camera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            camera->Update();
            AddLog("Camera position is now on the origin", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }
}

void UI::ConsoleLog()
{
    ImGui::Begin("Console");
    for (auto s : m_Logs)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, s.second);
        ImGui::Text(s.first.c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}

void UI::SceneRender()
{
    ImGui::Begin("Scene", nullptr);
    ImGui::Image((void *)m_FBO->GetColorBuffer(),
                 ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowWidth() / m_AspectRatio), // keep aspect ratio even if resize
                 ImVec2(0, 1),
                 ImVec2(1, 0));
    ImGui::End();
}

void UI::SetSelectedMesh(std::shared_ptr<Mesh> mesh)
{
    if (m_SelectedMesh != nullptr)
        m_SelectedMesh->m_IsSelected = false;
    m_SelectedMesh = mesh;
    m_SelectedMesh->m_IsSelected = true;
}

void UI::AddLog(const std::string log, ImVec4 color)
{
    m_Logs.insert(m_Logs.begin(), std::make_pair(log, color));
}
