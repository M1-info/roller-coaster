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

    // frame rate window
    FrameRate(viewport);

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
    ImGui::Begin("Scene Info");
    SceneGraph();
    MeshInfo();
    ImGui::End();
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
        // Object name
        ImGui::Text(m_SelectedMesh->GetName().c_str());
        ImGui::Separator();
        ImGui::Text("Transform");
        ImGui::Separator();
        MeshTransform("Position", m_SelectedMesh->m_Position);
        MeshTransform("Rotation", m_SelectedMesh->m_Rotation);
        MeshTransform("Scale", m_SelectedMesh->m_Scale);
    }
    ImGui::End();
}

void UI::MeshTransform(std::string component, glm::vec3 &value)
{

    ImGui::PushID(component.c_str());

    float reset = 0.0f;

    ImGui::Columns(2);
    ImGui::Text(component.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(70);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if(ImGui::Button("X"))
        value.x = reset;
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
        value.y = reset;
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
        value.z = reset;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &value.z, 0.1f);
    ImGui::PopItemWidth();
    ImGui::SameLine();


    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::PopID();

}