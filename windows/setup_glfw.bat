@echo off
chcp 65001 >nul
setlocal EnableDelayedExpansion

echo.
echo ==========================================
echo   SETUP GLFW - Simulasi Ruangan 3D
echo ==========================================
echo.
echo Script ini akan mendownload dan setup GLFW
echo untuk Windows secara otomatis.
echo.

REM Pindah ke root folder proyek
cd /d "%~dp0\.."

REM Cek apakah curl tersedia
where curl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] curl tidak ditemukan!
    echo curl biasanya sudah ada di Windows 10/11
    echo.
    echo Silakan download GLFW secara manual:
    echo   https://www.glfw.org/download.html
    echo.
    pause
    exit /b 1
)

REM Buat folder lib/glfw jika belum ada
if not exist "lib\glfw" mkdir lib\glfw

REM Download GLFW
set GLFW_URL=https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
set GLFW_ZIP=lib\glfw\glfw.zip

echo [1/3] Mendownload GLFW 3.4...
curl -L -o "%GLFW_ZIP%" "%GLFW_URL%"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Download gagal!
    pause
    exit /b 1
)

echo.
echo [2/3] Mengekstrak file...

REM Gunakan PowerShell untuk ekstrak
powershell -command "Expand-Archive -Path '%GLFW_ZIP%' -DestinationPath 'lib\glfw\temp' -Force"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Ekstrak gagal!
    pause
    exit /b 1
)

echo.
echo [3/3] Menyusun struktur folder...

REM Pindahkan file ke lokasi yang benar
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\include" (
    xcopy /E /I /Y "lib\glfw\temp\glfw-3.4.bin.WIN64\include" "lib\glfw\include"
)
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-mingw-w64" (
    xcopy /E /I /Y "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-mingw-w64" "lib\glfw\lib-mingw-w64"
)
if exist "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-vc2022" (
    xcopy /E /I /Y "lib\glfw\temp\glfw-3.4.bin.WIN64\lib-vc2022" "lib\glfw\lib-vc2022"
)

REM Cleanup
rd /s /q "lib\glfw\temp" 2>nul
del "%GLFW_ZIP%" 2>nul

echo.
echo ==========================================
echo   SETUP GLFW SELESAI!
echo ==========================================
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
