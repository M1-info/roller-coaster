project "imgui"
    kind "StaticLib"
    language "C++"
    files { "**.h", "**.cpp" }

    includedirs { "../glfw/include" }

    filter "action:vs*"
        targetdir "bin/VisualStudio/%{cfg.buildcfg}"
        objdir "bin-int/VisualStudio/%{cfg.buildcfg}"

    filter "action:gmake*"
        targetdir "bin/MinGW/%{cfg.buildcfg}"
        objdir "bin-int/MinGW/%{cfg.buildcfg}"