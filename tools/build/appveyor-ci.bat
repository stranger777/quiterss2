@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set PATH=%QTDIR%\bin;%CD%\bin;%PATH%

mkdir build
cd build
qmake CONFIG+=release -spec win32-msvc ..\quiterss2.pro
if %errorlevel% neq 0 exit /b %errorlevel%
nmake
if %errorlevel% neq 0 exit /b %errorlevel%
cd ..
echo === windeployqt verbose output ===
windeployqt build\bin --qmldir=%APPVEYOR_BUILD_FOLDER%\resources\qml --webengine --no-compiler-runtime --verbose 2
echo === deployment directory listing ===
dir build\bin /s /b
echo === QML plugins check ===
dir build\bin\qml /s /b 2>nul || echo No qml dir found
