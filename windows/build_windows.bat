@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

echo.
echo ==========================================
echo   BUILD SCRIPT - Simulasi Ruangan 3D
echo   Windows Version (MinGW)
echo ==========================================
echo.

REM Pindah ke root folder proyek
cd /d "%~dp0\.."

REM Cek apakah folder GLFW ada
if not exist "lib\glfw\include\GLFW\glfw3.h" (
    echo [ERROR] GLFW tidak ditemukan!
    echo.
    echo Silakan download GLFW terlebih dahulu:
    echo   1. Kunjungi: https://www.glfw.org/download.html
    echo   2. Download "Windows pre-compiled binaries" 64-bit
    echo   3. Ekstrak ke folder lib\glfw\
    echo.
    echo Struktur yang dibutuhkan:
    echo   lib\glfw\include\GLFW\glfw3.h
    echo   lib\glfw\lib-mingw-w64\libglfw3.a
    echo.
    pause
    exit /b 1
)

REM Cek MinGW
where mingw32-make >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] MinGW tidak ditemukan di PATH!
    echo.
    echo Silakan install MinGW:
    echo   1. Download dari: https://www.mingw-w64.org/downloads/
    echo   2. Atau gunakan MSYS2: https://www.msys2.org/
    echo   3. Pastikan bin folder ada di PATH
    echo.
    pause
    exit /b 1
)

REM Cek CMake
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake tidak ditemukan di PATH!
    echo.
    echo Silakan install CMake:
    echo   1. Download dari: https://cmake.org/download/
    echo   2. Centang "Add CMake to PATH" saat instalasi
    echo.
    pause
    exit /b 1
)

echo [INFO] Semua dependencies ditemukan!
echo.

REM Buat folder build jika belum ada
if not exist "build_win" mkdir build_win
cd build_win

echo [1/2] Menjalankan CMake...
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake gagal!
    pause
    exit /b 1
)

echo.
echo [2/2] Kompilasi dengan MinGW...
mingw32-make -j4
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==========================================
    echo   BUILD GAGAL!
    echo ==========================================
    pause
    exit /b 1
)

echo.
echo ==========================================
echo   BUILD BERHASIL!
echo ==========================================
echo.
echo Executable: build_win\RoomSimulation3D.exe
echo.
echo Untuk menjalankan, gunakan:
echo   - Double-click windows\run_windows.bat
echo   - Atau jalankan langsung: build_win\RoomSimulation3D.exe
echo.

cd ..
pause
