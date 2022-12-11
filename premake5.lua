workspace "Roller Coaster"
configurations { "Debug", "Release" }
platforms { "x64" }

startproject "app"

include "include/glad"
include "include/imgui"
include "include/stb_image"
include "app"