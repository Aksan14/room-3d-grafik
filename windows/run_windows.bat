@echo off
chcp 65001 >nul

echo ==========================================
echo   RUN - Simulasi Ruangan 3D
echo ==========================================
echo.

REM Pindah ke root folder proyek
cd /d "%~dp0\.."

REM Cari executable di berbagai lokasi (MinGW dan Visual Studio)
if exist "build_win\RoomSimulation3D.exe" (
    echo Menjalankan dari build_win...
    cd build_win
    start "" RoomSimulation3D.exe
    exit /b 0
)

if exist "build_win\Release\RoomSimulation3D.exe" (
    echo Menjalankan dari build_win\Release...
    cd build_win\Release
    start "" RoomSimulation3D.exe
    exit /b 0
)

if exist "build_win\Debug\RoomSimulation3D.exe" (
    echo Menjalankan dari build_win\Debug...
    cd build_win\Debug
    start "" RoomSimulation3D.exe
    exit /b 0
)

if exist "build\RoomSimulation3D.exe" (
    echo Menjalankan dari build...
    cd build
    start "" RoomSimulation3D.exe
    exit /b 0
)

if exist "build\Debug\RoomSimulation3D.exe" (
    echo Menjalankan dari build\Debug...
    cd build\Debug
    start "" RoomSimulation3D.exe
    exit /b 0
)

if exist "build\Release\RoomSimulation3D.exe" (
    echo Menjalankan dari build\Release...
    cd build\Release
    start "" RoomSimulation3D.exe
    exit /b 0
)

echo [ERROR] Executable tidak ditemukan!
echo.
echo Silakan build terlebih dahulu dengan:
echo   - MinGW: jalankan windows\build_windows.bat
echo   - Visual Studio: jalankan windows\build_windows_vs.bat
echo.
pause
