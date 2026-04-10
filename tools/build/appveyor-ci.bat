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
windeployqt build\bin --qmldir=resources\qml --no-compiler-runtime
