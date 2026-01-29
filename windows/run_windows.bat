@echo off
echo ==========================================
echo   RUN - Simulasi Ruangan 3D
echo ==========================================

cd ..

if exist "build\RoomSimulation3D.exe" (
    cd build
    RoomSimulation3D.exe
) else if exist "build\Debug\RoomSimulation3D.exe" (
    cd build\Debug
    RoomSimulation3D.exe
) else if exist "build\Release\RoomSimulation3D.exe" (
    cd build\Release
    RoomSimulation3D.exe
) else (
    echo Executable tidak ditemukan!
    echo Jalankan build_windows.bat terlebih dahulu.
    pause
)
