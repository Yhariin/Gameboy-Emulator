@echo off

:: Run from script’s own folder
cd /D "%~dp0"
setlocal

:: ─── Parse arguments ─────────────────────────────────────────────────────────
:: Default to debug clang, override if any arg is “release” or "msvc"
set "config=debug"
set "runFlag=0"
set "compiler=clang"
set "build_exe=0"
set "build_dll=0"
for %%A in (%*) do (
    if /I "%%A"=="release" set "config=release"
    if /I "%%A"=="debug" set "config=debug"
    if /I "%%A"=="run" set "runFlag=1"
    if /I "%%A"=="msvc" set "compiler=msvc"
    if /I "%%A"=="clang" set "compiler=clang"
    if /I "%%A"=="exe" set "build_exe=1"
    if /I "%%A"=="dll" set "build_dll=1"
)

echo [%compiler% %config% build]

:: ─── Build command setup ────────────────────────────────────────────────────
if /I "%config%"=="debug" if /I "%compiler%"=="clang" set "compile_link=  -Xlinker /SUBSYSTEM:CONSOLE "
if /I "%config%"=="debug" if /I "%compiler%"=="msvc" set "compile_link= /link /SUBSYSTEM:CONSOLE "
if /I "%config%"=="release" if /I "%compiler%"=="clang" set "compile_link= -Xlinker /SUBSYSTEM:WINDOWS -Xlinker /ENTRY:mainCRTStartup "
if /I "%config%"=="release" if /I "%compiler%"=="msvc" set "compile_link= /link /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup "

if /I "%config%"=="debug" if /I "%compiler%"=="clang" set "compile_cmd=clang++ -g -O0 -DDEBUG=1 "
if /I "%config%"=="debug" if /I "%compiler%"=="msvc" set "compile_cmd=cl /Od /Zi /DDEBUG=1 "
if /I "%config%"=="release" if /I "%compiler%"=="clang" set "compile_cmd=clang++ -O2 -DDEBUG=0 "
if /I "%config%"=="release" if /I "%compiler%"=="msvc" set "compile_cmd=cl /O2 /DDEBUG=0 "

if /I "%compiler%"=="clang" set "common_flags= -I../src/ -DOS_WINDOWS=1 -DCOMPILER_CLANG=1 -Wall -std=c++20 -Wno-unused-function -Wno-missing-braces -Wmissing-field-initializers -Wno-macro-redefined"
if /I "%compiler%"=="msvc" set "common_flags= /I../src/ /DOS_WINDOWS=1 /DCOMPILER_MSVC=1 /W3 /nologo /std:c++20 /Zc:preprocessor"

if /I "%compiler%"=="clang" set "out=-o"
if /I "%compiler%"=="msvc" set "out=/OUT:"

:: ─── Build ───────────────────────────────────────────────────────────────────
if not exist build mkdir build
pushd build

if /I "%build_dll%"=="1" (
    %compile_cmd% %common_flags% -shared ..\src\application.cpp  %out%application.dll
)

if /I "%build_exe%"=="1" (
    %compile_cmd% %common_flags% ..\src\main.cpp %compile_link% %out%emulator.exe
)

set "buildErr=%errorlevel%"
popd

if %buildErr% NEQ 0 (
    echo [31mBuild FAILED: exit code %buildErr%[0m
    exit /b %buildErr%
)

echo [32mBuild SUCCEEDED[0m

:: ─── Optional run ────────────────────────────────────────────────────────────
if "%runFlag%"=="1" (
    echo Running...
    echo.
    build\emulator.exe
)

exit /b 0