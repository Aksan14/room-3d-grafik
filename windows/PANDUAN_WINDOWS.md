# 🪟 PANDUAN INSTALASI DI WINDOWS

## 📋 Prasyarat

### Pilihan 1: MinGW (Lebih Mudah)
1. **Download MinGW-w64**
   - Kunjungi: https://www.mingw-w64.org/downloads/
   - Atau gunakan MSYS2: https://www.msys2.org/
   
2. **Download CMake**
   - Kunjungi: https://cmake.org/download/
   - Download installer Windows (.msi)
   - ✅ Centang "Add CMake to PATH" saat instalasi

### Pilihan 2: Visual Studio (Lebih Lengkap)
1. **Download Visual Studio Community**
   - Kunjungi: https://visualstudio.microsoft.com/
   - Pilih "Desktop development with C++"

---

## 📥 Download Library

### 1. GLAD (Sudah ada di lib/)
Jika belum ada:
- Kunjungi: https://glad.dav1d.de/
- Pengaturan:
  - Language: **C/C++**
  - Specification: **OpenGL**
  - API gl: **Version 3.3**
  - Profile: **Core**
  - ✅ Generate a loader
- Klik **GENERATE** dan download
- Letakkan:
  - `glad.h` → `lib/glad/include/glad/`
  - `khrplatform.h` → `lib/glad/include/KHR/`
  - `glad.c` → `lib/glad/src/`

### 2. GLM (Sudah ada di lib/)
Jika belum ada:
- Download: https://github.com/g-truc/glm/releases
- Ekstrak folder `glm` ke `lib/glm/`

### 3. GLFW untuk Windows ⚠️ WAJIB DOWNLOAD
- Download: https://www.glfw.org/download.html
- Pilih **Windows pre-compiled binaries** (64-bit)
- Ekstrak dan letakkan:
  - `include/GLFW/` → `lib/glfw/include/GLFW/`
  - `lib-mingw-w64/` → `lib/glfw/lib-mingw-w64/` (untuk MinGW)
  - `lib-vc2022/` → `lib/glfw/lib-vc2022/` (untuk Visual Studio)

---

## 📁 Struktur Folder untuk Windows

```
RoomSimulation3D/
├── lib/
│   ├── glad/           ✅ Sudah ada
│   ├── glm/            ✅ Sudah ada
│   └── glfw/           ⚠️ DOWNLOAD untuk Windows
│       ├── include/
│       │   └── GLFW/
│       │       ├── glfw3.h
│       │       └── glfw3native.h
│       ├── lib-mingw-w64/
│       │   ├── libglfw3.a
│       │   └── libglfw3dll.a
│       └── lib-vc2022/
│           └── glfw3.lib
├── shaders/            ✅ Sudah ada
├── src/                ✅ Sudah ada
├── windows/            📁 Folder ini
│   ├── build_windows.bat
│   ├── build_windows_vs.bat
│   ├── run_windows.bat
│   ├── CMakeLists_Windows.txt
│   └── PANDUAN_WINDOWS.md
└── CMakeLists.txt      (untuk Linux)
```

---

## 🔧 Cara Build di Windows

### Langkah 1: Siapkan CMakeLists.txt
Copy `CMakeLists_Windows.txt` ke root folder dan rename:
```cmd
copy windows\CMakeLists_Windows.txt CMakeLists.txt
```

### Langkah 2: Build

#### Menggunakan MinGW:
```cmd
cd windows
build_windows.bat
```

#### Menggunakan Visual Studio:
```cmd
cd windows
build_windows_vs.bat
```
Lalu buka `build/RoomSimulation3D.sln` dengan Visual Studio

---

## ▶️ Cara Run

```cmd
cd windows
run_windows.bat
```

Atau langsung:
```cmd
build\RoomSimulation3D.exe
```

---

## ⚠️ Troubleshooting

### Error: "glfw3.h not found"
- Pastikan GLFW sudah didownload dan diletakkan di `lib/glfw/`

### Error: "cannot find -lglfw3"
- Pastikan file `libglfw3.a` (MinGW) atau `glfw3.lib` (VS) ada di folder yang benar

### Error: "opengl32.lib not found"
- Library ini sudah include di Windows secara default

### Program crash / shader not found
- Pastikan folder `shaders/` ada di samping executable
- Build ulang agar shaders otomatis dicopy

### Error: "GLFW_INCLUDE_NONE not defined"
- Tambahkan `#define GLFW_INCLUDE_NONE` sebelum `#include <GLFW/glfw3.h>`

---

## 🎮 Kontrol Program

| Tombol | Fungsi |
|--------|--------|
| WASD | Gerak kamera |
| Mouse | Rotasi pandangan |
| Scroll | Zoom in/out |
| 1 | View depan |
| 2 | View atas |
| 3 | View samping |
| 4 | View sudut |
| L | Toggle lampu |
| ESC | Keluar |
