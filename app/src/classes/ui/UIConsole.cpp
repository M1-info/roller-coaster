#include "ui/UIConsole.h"

UIConsole::UIConsole()
{
}

UIConsole::~UIConsole()
{
}

void UIConsole::AddLog(const std::string log, ImVec4 color)
{
    // get current hours and minutes
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %Hh:%Mm:%Ss");
    auto time = oss.str();
    m_Logs.insert(m_Logs.begin(), {time, {log, color}});
}

void UIConsole::ConsoleWindow()
{
    ImGui::Begin("Console logs", nullptr, ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::Text("Console logs");
    ImGui::PopFont();

    ImGui::Separator();

    // Display logs
    for (auto s : m_Logs)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, s.second.second);
        std::string text = s.first + std::string(" - ") + s.second.first;
        ImGui::Text(text.c_str());
        ImGui::PopStyleColor();
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}