@echo off
echo ==========================================
echo   BUILD SCRIPT - Simulasi Ruangan 3D
echo   Windows Version
echo ==========================================

cd ..

if not exist "build" mkdir build
cd build

echo Menjalankan CMake...
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../windows/CMakeLists_Windows.txt ..

echo Kompilasi...
mingw32-make

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ==========================================
    echo   BUILD BERHASIL!
    echo ==========================================
    echo Jalankan dengan: build\RoomSimulation3D.exe
) else (
    echo.
    echo ==========================================
    echo   BUILD GAGAL!
    echo ==========================================
)

cd ..
pause
