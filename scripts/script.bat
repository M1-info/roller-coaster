@echo off

if "%1"=="" goto :EOF
if "%1"=="vs" goto :VS
if "%1"=="gmake" goto :GMAKE
if "%1"=="runDebug" goto :RUNDEBUG
if "%1"=="runRelease" goto :RUNRELEASE
if "%1"=="clean" goto :CLEAN

:VS
echo Generating Visual Studio project files...
pushd ..
./scripts/bin/premake5 vs2022
popd
exit /b

:GMAKE
echo Generating Makefiles...
pushd ..
./scripts/bin/premake5 gmake
popd
exit /b

:RUNDEBUG
echo Running in debug mode...
pushd ..
if exist ./app/bin/VisualStudio/Debug/ ./app/bin/VisualStudio/Debug/app.exe 
else ./app/bin/MinGW/Debug/app.exe
popd
exit /b

:RUNRELEASE
echo Running in release mode...
pushd ..
if exist ./app/bin/VisualStudio/Release/ ./app/bin/VisualStudio/Release/app.exe 
else ./app/bin/MinGW/Release/app.exe
popd
exit /b

:CLEAN
echo Cleaning...
pushd ..
rm -r -i ./app/bin
rm -r -i ./app/bin-int
rm -r -i ./imgui/bin
rm -r -i ./imgui/bin-int
rm -r -i ./glad/bin
rm -r -i ./glad/bin-int
rm -i ./sln
rm -i ./app/.vcxproj
rm -i ./app/.vcxproj.filters
rm -i ./app/.vcxproj.user
rm -i ./imgui/.vcxproj
rm -i ./imgui/.vcxproj.filters
rm -i ./imgui/.vcxproj.user
rm -i ./glad/.vcxproj
rm -i ./glad/.vcxproj.filters
rm -i ./glad/.vcxproj.user
rm -i ./app/Makefile
rm -i ./imgui/Makefile
rm -i ./glad/Makefile
rm -i ./Makefile
popd
exit /b

:EOF
echo Usage: script [vs|gmake|runDebug|runRelease|clean]
exit /b
