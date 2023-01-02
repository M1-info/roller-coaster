#pragma once
#ifndef UIFILESCONTROLLER_H
#define UIFILESCONTROLLER_H

#if VISUAL_STUDIO
#define FILEPATH_RAILS "src/assets/rails/"
#define FILEPATH_FONTS "src/assets/fonts/"
#elif MINGW
#define FILEPATH_RAILS "app\\src\\assets\\rails\\"
#define FILEPATH_FONTS "app\\src\\assets\\fonts\\"
#endif

#ifndef FILEPATH_RAILS
#define FILEPATH_RAILS "src/assets/rails/"
#endif

#ifndef FILEPATH_FONTS
#define FILEPATH_FONTS "src/assets/fonts/"
#endif

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <string>
#include <memory>
#include <vector>

#include "UIConsole.h"

#include <meshes/Rails.h>

class UIFilesController
{

public:
    UIFilesController(std::shared_ptr<UIConsole> UIConsole);
    ~UIFilesController();

    inline void SetSelectedFile(std::string file) { m_FileSelected = file; }

    void SelectFileWindow();
    void LoadFileWindow(std::shared_ptr<Rails> rails);
    void SaveFileWindow(std::shared_ptr<Rails> rails);
    void DeleteFileWindow();

    /**
     * @brief Load the control points of the selected files
     *
     */
    void LoadFile(std::shared_ptr<Rails> rails);

    /**
     * @brief Save the control points of the rails
     *
     */
    void SaveFile(std::vector<std::shared_ptr<ControlPoint>> controlPoints);

    /**
     * @brief Delete the selected file
     *
     */
    void DeleteFile();

    /**
     * @brief Load all files in the directory in the UI Combo Box
     *
     */
    void LoadFiles();

private:
    std::string m_FileName = "controlPoints";
    std::vector<std::string> m_Files;
    std::string m_FileSelected = "";

    std::shared_ptr<UIConsole> m_UIConsole;
};

#endif // UIFILESCONTROLLER_H