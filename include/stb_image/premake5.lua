project "stb_image"
    kind "StaticLib"
    language "C++"
    files { "./stb_image.cpp" }

    includedirs { "./"}

    filter "action:vs*"
        targetdir "bin/VisualStudio/%{cfg.buildcfg}"
        objdir "bin-int/VisualStudio/%{cfg.buildcfg}"

    filter "action:gmake*"
        targetdir "bin/MinGW/%{cfg.buildcfg}"
        objdir "bin-int/MinGW/%{cfg.buildcfg}"