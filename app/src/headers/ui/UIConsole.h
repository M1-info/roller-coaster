#pragma once
#ifndef UICONSOLE_H
#define UICONSOLE_H

#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "imgui/imgui.h"

class UIConsole
{
public:
    UIConsole();
    ~UIConsole();

    void ConsoleWindow();

    void AddLog(const std::string log, ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

private:
    std::map<std::string, std::pair<std::string, ImVec4>> m_Logs;
};

#endif // UICONSOLE_H