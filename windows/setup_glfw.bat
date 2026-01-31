@echo off
setlocal EnableDelayedExpansion

echo.
echo ==========================================
echo   SETUP GLFW - Simulasi Ruangan 3D
echo ==========================================
echo.

REM Pindah ke root folder proyek
cd /d "%~dp0\.."
echo Lokasi proyek: %CD%
echo.

REM Cek apakah GLFW sudah ada
if exist "lib\glfw\include\GLFW\glfw3.h" (
    echo [INFO] GLFW sudah terinstall!
    echo Lokasi: lib\glfw\include\GLFW\glfw3.h
    echo.
    echo Jika ingin reinstall, hapus folder lib\glfw terlebih dahulu.
    pause
    exit /b 0
)

echo Script ini akan mendownload GLFW 3.4 secara otomatis.
echo.

REM Cek apakah curl tersedia
where curl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] curl tidak ditemukan!
    echo.
    echo Solusi: Download GLFW secara manual:
    echo   1. Buka: https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
    echo   2. Ekstrak ke folder: lib\glfw\
    echo   3. Pastikan struktur: lib\glfw\include\GLFW\glfw3.h
    echo.
    pause
    exit /b 1
)

REM Buat folder jika belum ada
if not exist "lib" mkdir lib
if not exist "lib\glfw" mkdir lib\glfw

REM Download GLFW
set "GLFW_URL=https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip"
set "GLFW_ZIP=lib\glfw\glfw-download.zip"

echo [1/4] Mendownload GLFW 3.4 dari GitHub...
echo URL: %GLFW_URL%
echo.
curl -L --progress-bar -o "%GLFW_ZIP%" "%GLFW_URL%"
if not exist "%GLFW_ZIP%" (
    echo [ERROR] Download gagal! File tidak ditemukan.
    echo.
    echo Coba download manual dari:
    echo   https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
    pause
    exit /b 1
)

echo.
echo [2/4] Mengekstrak file ZIP...

REM Hapus folder temp jika ada
if exist "lib\glfw\temp" rd /s /q "lib\glfw\temp"

REM Gunakan PowerShell untuk ekstrak
powershell -NoProfile -ExecutionPolicy Bypass -Command "Expand-Archive -LiteralPath '%GLFW_ZIP%' -DestinationPath 'lib\glfw\temp' -Force"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Ekstrak dengan PowerShell gagal!
    echo Coba ekstrak manual file: %GLFW_ZIP%
    pause
    exit /b 1
)

echo.
echo [3/4] Menyusun struktur folder...

REM Cek apakah folder hasil ekstrak ada
if not exist "lib\glfw\temp\glfw-3.4.bin.WIN64" (
    echo [ERROR] Folder hasil ekstrak tidak ditemukan!
    echo Coba ekstrak manual file ZIP ke lib\glfw\
    pause
    exit /b 1
)

REM Copy include
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\include" (
    echo   Copying include...
    xcopy /E /I /Y /Q "lib\glfw\temp\glfw-3.4.bin.WIN64\include" "lib\glfw\include" >nul
)

REM Copy lib-mingw-w64
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-mingw-w64" (
    echo   Copying lib-mingw-w64...
    xcopy /E /I /Y /Q "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-mingw-w64" "lib\glfw\lib-mingw-w64" >nul
)

REM Copy lib-vc2022
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-vc2022" (
    echo   Copying lib-vc2022...
    xcopy /E /I /Y /Q "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-vc2022" "lib\glfw\lib-vc2022" >nul
)

echo.
echo [4/4] Membersihkan file temporary...

REM Cleanup
rd /s /q "lib\glfw\temp" 2>nul
del "%GLFW_ZIP%" 2>nul

REM Verifikasi instalasi
echo.
if exist "lib\glfw\include\GLFW\glfw3.h" (
    echo ==========================================
    echo   SETUP GLFW BERHASIL!
    echo ==========================================
    echo.
    echo File terinstall:
    if exist "lib\glfw\include\GLFW\glfw3.h" echo   [OK] lib\glfw\include\GLFW\glfw3.h
    if exist "lib\glfw\lib-mingw-w64\libglfw3.a" echo   [OK] lib\glfw\lib-mingw-w64\libglfw3.a
    if exist "lib\glfw\lib-vc2022\glfw3.lib" echo   [OK] lib\glfw\lib-vc2022\glfw3.lib
    echo.
    echo Selanjutnya jalankan:
    echo   - MinGW: build_windows.bat
    echo   - Visual Studio: build_windows_vs.bat
) else (
    echo ==========================================
    echo   SETUP GAGAL!
    echo ==========================================
    echo.
    echo File glfw3.h tidak ditemukan setelah ekstrak.
    echo Silakan download dan ekstrak manual.
)
echo.
echo Struktur folder GLFW:
echo   lib\glfw\include\GLFW\
echo   lib\glfw\lib-mingw-w64\  (untuk MinGW)
echo   lib\glfw\lib-vc2022\     (untuk Visual Studio)
echo.
echo Sekarang Anda bisa build dengan:
echo   - MinGW: windows\build_windows.bat
echo   - Visual Studio: windows\build_windows_vs.bat
echo.

pause
