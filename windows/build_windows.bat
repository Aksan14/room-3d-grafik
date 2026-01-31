@echo off
setlocal EnableDelayedExpansion

echo.
echo ==========================================
echo   BUILD SCRIPT - Simulasi Ruangan 3D
echo   Windows Version (MinGW)
echo ==========================================
echo.

REM Pindah ke root folder proyek
cd /d "%~dp0\.."
echo Lokasi proyek: %CD%
echo.

REM ========================================
REM CEK DEPENDENCIES
REM ========================================
echo [CHECKING] Memeriksa dependencies...
echo.

REM Cek CMake
echo   Checking CMake...
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo   [X] CMake TIDAK DITEMUKAN!
    echo.
    echo   SOLUSI:
    echo   1. Download CMake: https://cmake.org/download/
    echo   2. Saat install, WAJIB centang "Add CMake to PATH"
    echo   3. Restart Command Prompt setelah install
    echo.
    pause
    exit /b 1
)
echo   [OK] CMake ditemukan

REM Cek MinGW (gcc)
echo   Checking MinGW (gcc)...
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo   [X] MinGW (gcc) TIDAK DITEMUKAN!
    echo.
    echo   SOLUSI - Pilih salah satu:
    echo.
    echo   Cara 1 - WinLibs (RECOMMENDED):
    echo   1. Download: https://winlibs.com/
    echo   2. Pilih: Release ^> UCRT ^> Win64 ^> without LLVM
    echo   3. Ekstrak ke C:\mingw64
    echo   4. Tambahkan C:\mingw64\bin ke System PATH
    echo.
    echo   Cara 2 - MSYS2:
    echo   1. Download: https://www.msys2.org/
    echo   2. Install, lalu buka MSYS2 UCRT64
    echo   3. Jalankan: pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake
    echo   4. Tambahkan C:\msys64\ucrt64\bin ke System PATH
    echo.
    pause
    exit /b 1
)
echo   [OK] MinGW (gcc) ditemukan

REM Cek mingw32-make atau make
echo   Checking make...
set "MAKE_CMD="
where mingw32-make >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set "MAKE_CMD=mingw32-make"
    echo   [OK] mingw32-make ditemukan
) else (
    where make >nul 2>&1
    if %ERRORLEVEL% EQU 0 (
        set "MAKE_CMD=make"
        echo   [OK] make ditemukan
    )
)

if "%MAKE_CMD%"=="" (
    echo   [X] make TIDAK DITEMUKAN!
    echo.
    echo   make biasanya terinstall bersama MinGW.
    echo   Pastikan folder bin MinGW ada di PATH.
    echo.
    pause
    exit /b 1
)

REM Cek GLFW
echo   Checking GLFW...
if not exist "lib\glfw\include\GLFW\glfw3.h" (
    echo   [X] GLFW TIDAK DITEMUKAN!
    echo.
    echo   SOLUSI:
    echo   Jalankan: setup_glfw.bat
    echo.
    echo   Atau download manual:
    echo   1. https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
    echo   2. Ekstrak ke lib\glfw\
    echo   3. Pastikan ada: lib\glfw\include\GLFW\glfw3.h
    echo.
    pause
    exit /b 1
)
echo   [OK] GLFW ditemukan

if not exist "lib\glfw\lib-mingw-w64\libglfw3.a" (
    echo   [WARNING] libglfw3.a tidak ditemukan di lib-mingw-w64
    echo   Build mungkin gagal. Jalankan setup_glfw.bat ulang.
)

echo.
echo [OK] Semua dependencies tersedia!
echo.

REM ========================================
REM BUILD PROCESS
REM ========================================

REM Hapus folder build lama jika ada error sebelumnya
if exist "build_win\CMakeCache.txt" (
    echo [INFO] Membersihkan cache CMake lama...
    del /q "build_win\CMakeCache.txt" 2>nul
)

REM Buat folder build
if not exist "build_win" mkdir build_win
cd build_win

echo [1/2] Menjalankan CMake...
echo.
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] CMake gagal!
    echo.
    echo Kemungkinan penyebab:
    echo   - MinGW tidak terinstall dengan benar
    echo   - Konflik dengan compiler lain
    echo.
    cd ..
    pause
    exit /b 1
)

echo.
echo [2/2] Kompilasi dengan %MAKE_CMD%...
echo.
%MAKE_CMD% -j4
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ==========================================
    echo   BUILD GAGAL!
    echo ==========================================
    echo.
    echo Coba:
    echo   1. Hapus folder build_win
    echo   2. Jalankan build_windows.bat lagi
    echo.
    cd ..
    pause
    exit /b 1
)

cd ..

REM Verifikasi executable
if exist "build_win\RoomSimulation3D.exe" (
    echo.
    echo ==========================================
    echo   BUILD BERHASIL!
    echo ==========================================
    echo.
    echo Executable: %CD%\build_win\RoomSimulation3D.exe
    echo.
    echo Untuk menjalankan:
    echo   1. Double-click: windows\run_windows.bat
    echo   2. Atau langsung: build_win\RoomSimulation3D.exe
    echo.
) else (
    echo.
    echo [ERROR] Executable tidak ditemukan setelah build!
)

pause
