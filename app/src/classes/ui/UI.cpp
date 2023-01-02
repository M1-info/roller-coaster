#include "ui/UI.h"

UI::UI(std::shared_ptr<Window> window, std::shared_ptr<Scene> scene, std::shared_ptr<Light> light, std::shared_ptr<FrameBuffer> fbo)
    : m_Window(window), m_Light(light), m_Scene(scene)
{
    m_UIConsole = std::make_shared<UIConsole>();
    m_UIScene = new UIScene(scene, window, fbo, m_UIConsole);
}

UI::~UI()
{
    delete m_UIScene;
    m_UIScene = nullptr;
    delete m_UIConsole.get();
    m_UIConsole = nullptr;
}

void UI::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    std::string titleFont = FILEPATH_FONTS + std::string("Roboto-Black.ttf");
    m_Fonts["Title"] = io.Fonts->AddFontFromFileTTF(titleFont.c_str(), 22.0f);

    std::string subtitleFont = FILEPATH_FONTS + std::string("Roboto-Bold.ttf");
    m_Fonts["Subtitle"] = io.Fonts->AddFontFromFileTTF(subtitleFont.c_str(), 18.0f);

    std::string textFont = FILEPATH_FONTS + std::string("Roboto-Regular.ttf");
    m_Fonts["Text"] = io.Fonts->AddFontFromFileTTF(textFont.c_str(), 16.0f);

    m_UIScene->SetFonts(m_Fonts);

    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    io.FontDefault = m_Fonts["Text"];

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

    SetUpDockSpace();

    RenderWindow();
    LightWindow();
    CameraWindow();

    m_UIScene->SceneGraphWindow();
    if (m_UIScene->GetSelectedMesh() != nullptr)
        m_UIScene->SelectedMeshWindow();
    m_UIScene->SceneWindow();

    m_UIConsole->ConsoleWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::SetUpDockSpace()
{
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

    ImGuiID mainDockespace_id = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(mainDockespace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    static bool first = true;
    if (first)
    {
        ImGui::DockBuilderRemoveNode(mainDockespace_id);
        ImGui::DockBuilderAddNode(mainDockespace_id);

        auto dock_id_top = ImGui::DockBuilderSplitNode(mainDockespace_id, ImGuiDir_Up, 0.2f, nullptr, &mainDockespace_id);
        auto dock_id_down = ImGui::DockBuilderSplitNode(mainDockespace_id, ImGuiDir_Down, 0.15f, nullptr, &mainDockespace_id);
        auto dock_id_right = ImGui::DockBuilderSplitNode(mainDockespace_id, ImGuiDir_Right, 0.8f, nullptr, &mainDockespace_id);
        auto dock_id_left = ImGui::DockBuilderSplitNode(mainDockespace_id, ImGuiDir_Left, 0.2f, nullptr, &mainDockespace_id);

        ImGui::DockBuilderDockWindow("Scene", dock_id_right);
        ImGui::DockBuilderDockWindow("Scene Graph", dock_id_left);
        ImGui::DockBuilderDockWindow("Camera info", dock_id_left);
        ImGui::DockBuilderDockWindow("Selected mesh info", dock_id_left);
        ImGui::DockBuilderDockWindow("Console", dock_id_down);

        ImGui::DockBuilderFinish(mainDockespace_id);
        first = false;
        m_UIConsole->AddLog("Docking system initialized", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    }

    ImGui::End();
}

void UI::RenderWindow()
{
    ImGui::Begin("Render");

    // Is animation running
    ImGui::Checkbox("Animation", &m_IsAnimating);

    // swap camera position
    std::shared_ptr<Camera> camera = m_Window->GetCamera();
    std::shared_ptr<Cart> cart = std::dynamic_pointer_cast<Cart>(m_Scene->GetObjectByName("Cart"));
    std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_Scene->GetObjectByName("Rails"));

    if (ImGui::Checkbox("Swap camera position", &camera->m_IsOnCart))
        if (camera->m_IsOnCart)
        {
            glm::vec3 cartPosition = cart->GetTransform()->GetPosition();
            camera->SetPosition(cartPosition + glm::vec3(0.0f, 2.0f, 0.0f));
            m_UIConsole->AddLog("Camera is now locked on the cart", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
        else
        {
            camera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            camera->Update();
            m_UIConsole->AddLog("Camera is now unlocked from the cart", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }

    if (ImGui::Checkbox("Draw rails", &rails->m_DrawRails))
        if (rails->m_DrawRails)
        {
            if (rails->GetChildren().empty())
            {
                m_UIConsole->AddLog("No control points, draw not possible", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                rails->m_DrawRails = false;
            }
            else
            {
                rails->UpdateRails();
                m_UIConsole->AddLog("Rails drawn", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
        }

    // Frame Rate
    ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::End();
}

void UI::CameraWindow()
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

    // SwapCameraPosition();

    ImGui::End();

    ImGui::PopID();
}

void UI::LightWindow()
{
    glm::vec3 *position = m_Light->GetTransform()->GetPositionPtr();
    glm::vec3 *color = m_Light->GetColorPtr();
    float *intensity = m_Light->GetIntensityPtr();

    ImGui::PushID(m_Light.get());

    ImGui::Begin("Light");

    ImGui::Columns(2);

    ImGui::SetColumnWidth(0, ImGui::CalcTextSize("Position").x + 10);
    ImGui::Text("Position");

    ImGui::NextColumn();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0));
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

    const char *components[3] = {"X", "Y", "Z"};
    float *values[3] = {&position->x, &position->y, &position->z};
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
            *values[i] = 1.0f;
            m_Light->GetTransform()->SetIsDirty(true);
        }
        ImGui::PopStyleColor(2);

        ImGui::SameLine();

        // drag float
        std::string label = "##" + std::string(components[i]);
        ImGui::DragFloat(label.c_str(), values[i], 0.1f);

        ImGui::PopItemWidth();
        ImGui::SameLine();
    }

    ImGui::PopStyleVar();
    ImGui::Columns(1);

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::ColorEdit3("Color", (float *)color);

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Columns(2);

    ImGui::SetColumnWidth(0, ImGui::CalcTextSize("Intensity").x + 10);
    ImGui::Text("Intensity");

    ImGui::NextColumn();

    ImGui::DragFloat("##Intensity", intensity, 0.1f, 0.0f, 10.0f);

    ImGui::Columns(1);

    ImGui::End();

    ImGui::PopID();
}
