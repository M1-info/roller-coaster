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

    ConsoleLog();

    // scene infos window
    SceneInfo();

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

void UI::SceneInfo()
{
    SceneGraph();
    MeshInfo();
    MeshTransform("Light Position", m_Light->m_Position, 0.0f);
    CameraInfo();
}

void UI::SceneGraph()
{
    ImGui::Begin("Scene Graph");
    for (auto mesh: m_Scene->GetObjects())
    {
        if (ImGui::Selectable(mesh->GetName().c_str(), m_SelectedMesh == mesh))
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
        ImGui::Separator();
        MeshTransform("Position", m_SelectedMesh->m_Position);
        ImGui::Separator();
        MeshTransform("Rotation", m_SelectedMesh->m_Rotation);
        ImGui::Separator();
        MeshTransform("Scale", m_SelectedMesh->m_Scale, 1.0f);
    }
    ImGui::End();
}

void UI::MeshTransform(std::string component, glm::vec3 &value, float resetValue)
{

    ImGui::PushID(component.c_str());

    ImGui::Columns(2);
    ImGui::Text(component.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(70);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if(ImGui::Button("X"))
        value.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &value.x, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.9f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    if(ImGui::Button("Y"))
        value.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &value.y, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushItemWidth(70);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.9f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    if(ImGui::Button("Z"))
        value.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &value.z, 0.1f);
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
    ImGui::Text("Camera Mouvements");
    ImGui::NextColumn();

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

    ImGui::Text("Camera position");
    ImGui::NextColumn();

    ImGui::PushItemWidth(70);

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