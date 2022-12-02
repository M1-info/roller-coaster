project "app"
    kind "ConsoleApp"
    language "C++"
    files { "src/**.h", "src/**.cpp" }
    includedirs { "../include/glad/include", "../include/glfw/include", "../include" }
    
    links { "glad", "glfw3", "imgui", "opengl32", "gdi32", "user32", "shell32" }

    filter "action:vs*"
        systemversion "latest"
        ignoredefaultlibraries { "libmct" }
        libdirs "../include/glfw/lib-vc2022"
        targetdir "bin/VisualStudio/%{cfg.buildcfg}"
        objdir "bin-int/VisualStudio/%{cfg.buildcfg}"

    filter "action:gmake*"
        buildoptions { "-std=c++17" }
        libdirs "../include/glfw/lib-mingw-w64"
        targetdir "bin/MinGW/%{cfg.buildcfg}"
        objdir "bin-int/MinGW/%{cfg.buildcfg}"
    