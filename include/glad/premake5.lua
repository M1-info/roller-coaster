project "glad"
    kind "StaticLib"
    language "C"
    files { "include/**.h", "src/**.c" }
    includedirs { "include" }

    
    filter "action:vs*"
        targetdir "bin/VisualStudio/%{cfg.buildcfg}"
        objdir "bin-int/VisualStudio/%{cfg.buildcfg}"

    filter "action:gmake*"
        targetdir "bin/MinGW/%{cfg.buildcfg}"
        objdir "bin-int/MinGW/%{cfg.buildcfg}"