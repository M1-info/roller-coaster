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

    // Setup Dear ImGui context

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    // Add fonts
    std::string titleFont = FILEPATH_FONTS + std::string("Roboto-Black.ttf");
    m_Fonts["Title"] = io.Fonts->AddFontFromFileTTF(titleFont.c_str(), 22.0f);

    std::string subtitleFont = FILEPATH_FONTS + std::string("Roboto-Bold.ttf");
    m_Fonts["Subtitle"] = io.Fonts->AddFontFromFileTTF(subtitleFont.c_str(), 18.0f);

    std::string textFont = FILEPATH_FONTS + std::string("Roboto-Regular.ttf");
    m_Fonts["Text"] = io.Fonts->AddFontFromFileTTF(textFont.c_str(), 16.0f);

    m_UIScene->SetFonts(m_Fonts);

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_Window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    // Set default font
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

    m_UIScene->Render();

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

    // Main dockspace full screen
    ImGui::Begin("MainDockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    // Split dockspace in two parts
    static bool firstMain = true;
    if (firstMain)
    {
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

        ImGuiID dock_main_id = dockspace_id;
        ImGuiID dock_id_left, dock_id_right;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.85f, &dock_id_left, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.15f, &dock_id_right, &dock_main_id);

        // Add windows to dockspaces
        ImGui::DockBuilderDockWindow("Scene Console dockspace", dock_id_left);
        ImGui::DockBuilderDockWindow("Render dockspace", dock_id_right);

        ImGui::DockBuilderFinish(dockspace_id);
        firstMain = false;
    }
    ImGui::End();

    // Right dockspace for render informations window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Render dockspace", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleVar(3);

    ImGuiID render_dockspace_id = ImGui::GetID("Render dockspace");
    ImGui::DockSpace(render_dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar);

    static bool firstRender = true;
    if (firstRender)
    {
        ImGui::DockBuilderRemoveNode(render_dockspace_id);
        ImGui::DockBuilderAddNode(render_dockspace_id);
        ImGui::DockBuilderSetNodeSize(render_dockspace_id, viewport->Size);

        ImGuiID dock_main_id = render_dockspace_id;
        ImGuiID dock_id_bottom1, dock_id_bottom2, dock_id_top;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.4f, &dock_id_bottom1, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.4f, &dock_id_bottom2, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, &dock_id_top, &dock_main_id);

        ImGui::DockBuilderDockWindow("Render infos", dock_id_top);
        ImGui::DockBuilderDockWindow("Camera infos", dock_id_bottom1);
        ImGui::DockBuilderDockWindow("Light infos", dock_id_bottom2);

        ImGui::DockBuilderFinish(render_dockspace_id);
        firstRender = false;
    }
    ImGui::End();

    // Dockspace for scene, scene graph and console
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene Console dockspace", nullptr, ImGuiWindowFlags_NoTitleBar);
    ImGui::PopStyleVar(3);

    ImGuiID scene_console_dockspace_id = ImGui::GetID("Scene Console dockspace");
    ImGui::DockSpace(scene_console_dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_NoTabBar);

    static bool first_scene_console = true;
    if (first_scene_console)
    {
        ImGui::DockBuilderRemoveNode(scene_console_dockspace_id);
        ImGui::DockBuilderAddNode(scene_console_dockspace_id);

        ImGuiID dock_main_id = scene_console_dockspace_id;
        ImGuiID dock_id_bottom, dock_id_top;
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, &dock_id_bottom, &dock_main_id);
        ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.9f, &dock_id_top, &dock_main_id);

        ImGui::DockBuilderDockWindow("Scene dockspace", dock_id_top);
        ImGui::DockBuilderDockWindow("Console logs", dock_id_bottom);

        ImGui::DockBuilderFinish(scene_console_dockspace_id);
        first_scene_console = false;
    }
    ImGui::End();
}

void UI::RenderWindow()
{

    ImGui::PushID("Render window");

    ImGui::Begin("Render infos", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(m_Fonts["Title"]);
    ImGui::Text("Roaller Coaster Simulation");
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Text("Move the mouse and hold right click to orient the camera");
    ImGui::Text("Use the WASD keys to move the camera");

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Is animation running

    if (ImGui::Checkbox("Animation", &m_IsAnimating))
    {
        if (m_IsAnimating)
        {
            std::shared_ptr<Rails> rails = std::dynamic_pointer_cast<Rails>(m_Scene->GetObjectByName("Rails"));
            std::shared_ptr<Cart> cart = std::dynamic_pointer_cast<Cart>(m_Scene->GetObjectByName("Cart"));

            cart->SetCurves(rails->GetCurves());
            cart->SetCurrentCurveIndex(0);
            cart->SetCurveCount(rails->GetCurves().size());
        }
    }

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Swap camera position

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

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Change curve type

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Text("Curve type");
    ImGui::PopFont();

    ImGui::Dummy(ImVec2(0.0f, 1.0f));

    const char *curveTypes[] = {"Bezier", "Catmull-Rom", "B-Spline"};
    if (ImGui::BeginCombo("##", m_CurveSelected))
    {
        for (int n = 0; n < IM_ARRAYSIZE(curveTypes); n++)
        {
            bool is_selected = (m_CurveSelected == curveTypes[n]);
            if (ImGui::Selectable(curveTypes[n], is_selected))
            {
                m_CurveSelected = curveTypes[n];
                if (m_CurveSelected == "Bezier")
                    rails->m_CurveType = CurveType::BEZIER;
                else if (m_CurveSelected == "Catmull-Rom")
                    rails->m_CurveType = CurveType::CATMULL_ROM;
                else if (m_CurveSelected == "B-Spline")
                    rails->m_CurveType = CurveType::BSPLINE;

                rails->Update();
                m_UIConsole->AddLog("Curve type changed to " + std::string(m_CurveSelected), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Draw / Hide rails

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

    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    // Frame Rate
    ImGui::Text("Frame Rate: %.1f FPS", ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::PopID();
}

void UI::CameraWindow()
{
    std::shared_ptr<Camera> camera = m_Window->GetCamera();
    glm::vec3 *position = camera->GetPositionPtr();
    float *cameraSpeed = camera->GetMoveSpeedPtr();
    float *sensitivity = camera->GetMoveSensitivityPtr();

    ImGui::PushID(m_Light.get());

    ImGui::Begin("Camera infos", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(m_Fonts["Title"]);
    ImGui::Text("Camera");
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

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

    ImGui::Columns(2);

    // Camera speed
    ImGui::SetColumnWidth(0, ImGui::CalcTextSize("Speed").x + 10);
    ImGui::Text("Speed");

    ImGui::NextColumn();

    ImGui::DragFloat("##Speed", cameraSpeed, 0.1f, 0.0f, 10.0f);

    ImGui::Columns(1);

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Columns(2);

    // Camera sensitivity
    ImGui::SetColumnWidth(0, ImGui::CalcTextSize("Sensitivity").x + 10);
    ImGui::Text("Sensitivity");

    ImGui::NextColumn();

    ImGui::DragFloat("##Sensitivity", sensitivity, 0.1f, 0.0f, 10.0f);
    ImGui::Columns(1);

    ImGui::End();

    ImGui::PopID();
}

void UI::LightWindow()
{
    glm::vec3 *position = m_Light->GetTransform()->GetPositionPtr();
    glm::vec3 *color = m_Light->GetColorPtr();
    float *intensity = m_Light->GetIntensityPtr();

    ImGui::PushID(m_Light.get());

    ImGui::Begin("Light infos", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(m_Fonts["Title"]);
    ImGui::Text("Light");
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Columns(2);

    // Light position
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

    // Light color
    ImGui::ColorEdit3("Color", (float *)color);

    ImGui::Dummy(ImVec2(0.0f, 2.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 2.0f));

    ImGui::Columns(2);

    // Light intensity
    ImGui::SetColumnWidth(0, ImGui::CalcTextSize("Intensity").x + 10);
    ImGui::Text("Intensity");

    ImGui::NextColumn();

    ImGui::DragFloat("##Intensity", intensity, 0.1f, 0.0f, 100.0f);

    ImGui::Columns(1);

    ImGui::End();

    ImGui::PopID();
}
