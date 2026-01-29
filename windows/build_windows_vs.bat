@echo off
echo ==========================================
echo   BUILD SCRIPT - Simulasi Ruangan 3D
echo   Visual Studio Version
echo ==========================================

cd ..

if not exist "build" mkdir build
cd build

echo Menjalankan CMake untuk Visual Studio...
cmake -G "Visual Studio 17 2022" -A x64 ..

echo.
echo ==========================================
echo   CMake selesai!
echo ==========================================
echo Buka file: build\RoomSimulation3D.sln
echo dengan Visual Studio, lalu Build (Ctrl+Shift+B)

cd ..
pause
