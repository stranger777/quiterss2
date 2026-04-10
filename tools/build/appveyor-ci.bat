@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
set PATH=%QTDIR%\bin;%CD%\bin;%PATH%

mkdir build
cd build
qmake CONFIG+=release -spec win32-msvc DISABLE_BROWSER=true ..\quiterss2.pro
if %errorlevel% neq 0 exit /b %errorlevel%
nmake
if %errorlevel% neq 0 exit /b %errorlevel%
cd ..
windeployqt build\bin --qmldir=%APPVEYOR_BUILD_FOLDER%\resources\qml --no-compiler-runtime
