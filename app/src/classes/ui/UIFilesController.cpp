#include "ui/UIFilesController.h"

UIFilesController::UIFilesController(std::shared_ptr<UIConsole> UIConsole) : m_UIConsole(UIConsole)
{
    LoadFiles();
}

UIFilesController::~UIFilesController()
{
}

void UIFilesController::SelectFileWindow()
{
    if (ImGui::BeginCombo("Selected File", m_FileSelected.c_str()))
    {
        for (auto file : m_Files)
        {
            bool is_selected = (m_FileSelected == file);
            if (ImGui::Selectable(file.c_str(), is_selected))
            {
                m_FileSelected = file;
                std::string message = "File selected: " + m_FileSelected;
                m_UIConsole->AddLog(message.c_str(), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

void UIFilesController::SaveFileWindow(std::shared_ptr<Rails> rails)
{
    if (ImGui::Button("Save current points"))
    {
        std::vector<std::shared_ptr<Mesh>> children = rails->GetChildren();

        if (children.size() == 0)
        {
            m_UIConsole->AddLog("No control points to save", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            return;
        }

        // cast children to control points
        std::vector<std::shared_ptr<ControlPoint>> controlPoints;

        for (auto child : children)
        {
            std::shared_ptr<ControlPoint> controlPoint = std::dynamic_pointer_cast<ControlPoint>(child);
            controlPoints.push_back(controlPoint);
        }

        SaveFile(controlPoints);
        LoadFiles();
        m_FileName = "controlPoints";
    }
}

void UIFilesController::LoadFileWindow(std::shared_ptr<Rails> rails)
{
    if (ImGui::Button("Load file"))
    {
        if (m_FileSelected == "")
        {
            m_UIConsole->AddLog("No file selected", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        else
        {
            LoadFile(rails);
            m_FileSelected = "";
        }
    }
}

void UIFilesController::DeleteFileWindow()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    if (ImGui::Button("Delete file"))
    {
        if (m_FileSelected == "")
        {
            m_UIConsole->AddLog("No file selected", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        }
        else
        {
            DeleteFile();
            LoadFiles();
            m_FileSelected = "";
        }
    }
    ImGui::PopStyleColor(2);
}

void UIFilesController::LoadFiles()
{
    m_Files.clear();

    // load all files in directory
    for (const auto &entry : std::filesystem::directory_iterator(FILEPATH_RAILS))
    {
        std::string filename = entry.path().filename().string();
        m_Files.push_back(filename);
    }
}

void UIFilesController::SaveFile(std::vector<std::shared_ptr<ControlPoint>> controlPoints)
{
    std::string filename = m_FileName + ".txt";

    bool exists = true;
    int i = 0;
    struct stat buf;
    while (exists)
    {
        std::string file = FILEPATH_RAILS + filename;
        if (stat(file.c_str(), &buf) != -1)
        {
            i++;
            filename = m_FileName + "_" + std::to_string(i) + ".txt";
        }
        else
            exists = false;
    }

    m_FileName = FILEPATH_RAILS + filename;

    std::ofstream file(m_FileName);

    if (!file.is_open())
    {
        m_UIConsole->AddLog("Error saving file", ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        return;
    }

    for (auto point : controlPoints)
    {
        glm::vec3 vertex = point.get()->GetVertices()[0];
        file << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
    }

    file.close();

    std::string message = "File saved successfully: " + filename;
    m_UIConsole->AddLog(message.c_str(), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void UIFilesController::DeleteFile()
{

    std::string file = FILEPATH_RAILS + std::string(m_FileSelected);
    std::string message = "";
    ImVec4 color;

    if (remove(file.c_str()) != 0)
    {
        message = "Error deleting file" + m_FileSelected;
        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        message = "File " + m_FileSelected + " deleted successfully";
        color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
    m_UIConsole->AddLog(message.c_str(), color);
}

void UIFilesController::LoadFile(std::shared_ptr<Rails> rails)
{
    std::ifstream file(FILEPATH_RAILS + m_FileSelected);

    std::string message = "";
    ImVec4 color;

    if (!file.is_open())
    {
        message = "Error loading file " + m_FileSelected;
        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        m_UIConsole->AddLog(message.c_str(), color);
        return;
    }

    std::string line;

    std::vector<glm::vec3> controlPoints;

    while (std::getline(file, line))
    {
        glm::vec3 vertex;
        sscanf_s(line.c_str(), "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
        glm::vec3 point(vertex.x, vertex.y, vertex.z);
        controlPoints.push_back(point);
    }

    rails->GenerateControlPoints(controlPoints);
    rails->Update();

    message = "File " + m_FileSelected + " loaded successfully";
    color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    m_UIConsole->AddLog(message.c_str(), color);
}