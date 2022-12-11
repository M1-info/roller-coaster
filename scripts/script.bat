@echo off

if "%1"=="" goto :EOF
if "%1"=="help" goto :HELP
if "%1"=="vs" goto :VS
if "%1"=="gmake" goto :GMAKE
if "%1"=="runDebug" goto :RUNDEBUG
if "%1"=="runRelease" goto :RUNRELEASE
if "%1"=="clean" goto :CLEAN

:VS
echo Generating Visual Studio project files...
.\scripts\bin\premake5 vs2022
exit /b

:GMAKE
echo Generating Makefiles...
.\scripts\bin\premake5.exe gmake
exit /b

:RUNDEBUG
echo Running in debug mode...
if exist app\bin\VisualStudio\Debug\ app\bin\VisualStudio\Debug\app.exe 
else app\bin\MinGW\Debug\app.exe
exit /b

:RUNRELEASE
echo Running in release mode...
if exist app\bin\VisualStudio\Release\ app\bin\VisualStudio\Release\app.exe 
else app\bin\MinGW\Release\app.exe
exit /b

:CLEAN
echo Cleaning...
if exist *.sln rm -r *.sln

if exist app\app.vcxproj rm -r app\app.vcxproj
if exist app\app.vcxproj.filters rm -r app\app.vcxproj.filters
if exist app\app.vcxproj.user rm -r app\app.vcxproj.user

if exist include\imgui\imgui.vcxproj rm -r include\imgui\imgui.vcxproj
if exist include\imgui\imgui.vcxproj.filters rm -r include\imgui\imgui.vcxproj.filters
if exist include\imgui\imgui.vcxproj.user rm -r include\imgui\imgui.vcxproj.user

if exist include\glad\glad.vcxproj rm -r include\glad\glad.vcxproj
if exist include\glad\glad.vcxproj.filters rm -r include\glad\glad.vcxproj.filters
if exist include\glad\glad.vcxproj.user rm -r include\glad\glad.vcxproj.user

if exist include\stb_image\stb_image.vcxproj rm -r include\stb_image\stb_image.vcxproj
if exist include\stb_image\stb_image.vcxproj.filters rm -r include\stb_image\stb_image.vcxproj.filters
if exist include\stb_image\stb_image.vcxproj.user rm -r include\stb_image\stb_image.vcxproj.user

if exist app\Makefile rm -r app\Makefile
if exist include\imgui\Makefile rm -r include\imgui\Makefile
if exist include\glad\Makefile rm -r include\glad\Makefile
if exist include\stb_image\Makefile rm -r include\stb_image\Makefile

if exist app\bin rm -r app\bin
if exist app\bin-int rm -r app\bin-int
if exist include\imgui\bin rm -r include\imgui\bin
if exist include\imgui\bin-int rm -r include\imgui\bin-int
if exist include\glad\bin rm -r include\glad\bin
if exist include\glad\bin-int rm -r include\glad\bin-int
if exist include\stb_image\bin rm -r include\stb_image\bin
if exist include\stb_image\bin-int rm -r include\stb_image\bin-int

if exist ./Makefile rm -r ./Makefile
if exist ./*.sln rm -r ./*.sln
exit /b

:HELP
echo Usage: script.bat [command] 
echo Commands : 
echo vs - Generates Visual Studio project files
echo gmake - Generates Makefiles
echo runDebug - Runs in debug mode
echo runRelease - Runs in release mode
echo clean - Cleans the project
exit /b

:EOF
echo Usage: script.bat [vs|gmake|runDebug|runRelease|clean]
echo Use help to see commands or check the README file
exit /b
