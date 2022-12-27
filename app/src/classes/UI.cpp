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

    /* scene infos window */
    
    ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    SceneGraph();
    

    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y -600), ImGuiCond_Once);
    MeshInfo();
    
    ImGui::SetNextWindowSize(ImVec2(500, 150), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 400), ImGuiCond_Once);
    CameraInfo();

    
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetIO().DisplaySize.y - 250), ImGuiCond_Once);
    ConsoleLog();
    

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
    for (auto mesh: m_Scene->GetObjects())
    {
        if(mesh->GetChildren().size() > 0)
        {
            if (ImGui::TreeNode(mesh->GetName().c_str()))
            {
                for (auto child: mesh->GetChildren())
                {
                    if (ImGui::Selectable(child->GetName().c_str(), m_SelectedMesh == child))
                        SetSelectedMesh(child);
                }
                ImGui::TreePop();
            }
        }
        else if (ImGui::Selectable(mesh->GetName().c_str(), m_SelectedMesh == mesh))
            SetSelectedMesh(mesh);
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
        // ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Add some space between the two lines

        // if(m_SelectedMesh->GetType() == MeshType::CONTROL_POINT){
        //     glm::vec3 *vertex = m_SelectedMesh->GetVertexPtr(0);
        //     MeshTransform("Position", vertex);
        // }
        // else
        //     MeshTransform("Position", &m_SelectedMesh->m_Position, 0.0f);
        // ImGui::Separator();
        // MeshTransform("Rotation", &m_SelectedMesh->m_Rotation);
        // ImGui::Separator();
        // MeshTransform("Scale", &m_SelectedMesh->m_Scale, 1.0f);


        if(m_SelectedMesh->GetType() == MeshType::CONTROL_POINT){
            glm::vec3 *vertex = m_SelectedMesh->GetVertexPtr(0);
            MeshTransform("Position", vertex, glm::vec3(0.0f));
            ImGui::Separator();
            vertex = m_SelectedMesh->GetRotationPtr();
            MeshTransform("Rotation", vertex, glm::vec3(0.0f));
            ImGui::Separator();
            vertex = m_SelectedMesh->GetScalePtr();
            MeshTransform("Scale", vertex, glm::vec3(1.0f));
        } else {
            MeshTransform("Position", &m_SelectedMesh->m_Position, glm::vec3(0.0f));
            ImGui::Separator();
            MeshTransform("Rotation", &m_SelectedMesh->m_Rotation, glm::vec3(0.0f));
            ImGui::Separator();
            MeshTransform("Scale", &m_SelectedMesh->m_Scale, glm::vec3(1.0f));
        }

    }
    ImGui::End();
}

void UI::MeshTransform(std::string component, glm::vec3 *value, glm::vec3 resetValue)
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
    if(ImGui::Button("X"))
        value->x = resetValue.x;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if(ImGui::DragFloat("##X", &value->x, 0.1f))
        m_SelectedMesh->Update();

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if(ImGui::Button("Y"))
        value->y = resetValue.y;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if(ImGui::DragFloat("##Y", &value->y, 0.1f))
        m_SelectedMesh->Update();

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    if(ImGui::Button("Z"))
        value->z = resetValue.z;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    if(ImGui::DragFloat("##Z", &value->z, 0.1f))
        m_SelectedMesh->Update();
        
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
    ImGui::SetColumnWidth(0, 150);
    ImGui::Text("Camera Mouvements");
    ImGui::NextColumn();
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 2));
    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if(ImGui::Button("Speed"))
        *cameraSpeed = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Speed", cameraSpeed, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if(ImGui::Button("Sensitivity"))
        *sensitivity = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Sensitivity", sensitivity, 0.1f, 0.1f, 1.0f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::NextColumn();
    ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Add some space between the two lines

    ImGui::Text("Camera position");
    ImGui::NextColumn();

    ImGui::PushItemWidth(70);

    ImGui::Dummy(ImVec2(0.0f, 20.0f)); // Add some space between the two lines
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if(ImGui::Button("X"))
        position->x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if(ImGui::DragFloat("##X", &position->x, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if(ImGui::Button("Y"))
        position->y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if(ImGui::DragFloat("##Y", &position->y, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    if(ImGui::Button("Z"))
        position->z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if(ImGui::DragFloat("##Z", &position->z, 0.1f))
        camera->Update();
    ImGui::PopItemWidth();
    ImGui::SameLine();


    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::End();

    ImGui::PopID();
}

void UI::ConsoleLog()
{
    ImGui::Begin("Console");
    for(auto s: m_Logs)
        ImGui::Text(s.c_str());
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}