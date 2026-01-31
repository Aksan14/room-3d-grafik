# 🪟 PANDUAN INSTALASI DI WINDOWS

## � Quick Start (Cara Cepat)

### Langkah 1: Install Prerequisites
1. **CMake** - https://cmake.org/download/ (centang "Add to PATH")
2. **MinGW** - https://www.mingw-w64.org/downloads/ atau **Visual Studio 2022**

### Langkah 2: Download GLFW (Otomatis)
```cmd
cd windows
setup_glfw.bat
```
Atau download manual dari https://www.glfw.org/download.html

### Langkah 3: Build & Run
```cmd
cd windows
build_windows.bat      # Untuk MinGW
# atau
build_windows_vs.bat   # Untuk Visual Studio

run_windows.bat        # Jalankan program
```

---

## 📋 Prasyarat Detail

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

**Cara Otomatis:**
```cmd
cd windows
setup_glfw.bat
```

**Cara Manual:**
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
│   └── glfw/           ⚠️ Download dengan setup_glfw.bat
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
│   ├── setup_glfw.bat       🆕 Download GLFW otomatis
│   ├── build_windows.bat    Build dengan MinGW
│   ├── build_windows_vs.bat Build dengan Visual Studio
│   ├── run_windows.bat      Jalankan program
│   └── PANDUAN_WINDOWS.md
└── CMakeLists.txt      (Support Windows & Linux)
```

---

## 🔧 Cara Build di Windows

### Menggunakan MinGW:
```cmd
cd windows
build_windows.bat
```

### Menggunakan Visual Studio:
```cmd
cd windows
build_windows_vs.bat
```

---

## ▶️ Cara Menjalankan

```cmd
cd windows
run_windows.bat
```

Atau langsung jalankan executable:
- MinGW: `build_win\RoomSimulation3D.exe`
- Visual Studio: `build_win\Release\RoomSimulation3D.exe`

---

## ⚠️ Troubleshooting

### Error: "GLFW tidak ditemukan"
Jalankan `setup_glfw.bat` atau download manual dari https://www.glfw.org/download.html

### Error: "MinGW tidak ditemukan"
Pastikan MinGW sudah terinstall dan folder `bin` ada di PATH:
```cmd
set PATH=%PATH%;C:\mingw64\bin
```

### Error: "CMake tidak ditemukan"
Install CMake dan pastikan mencentang "Add to PATH" saat instalasi.

### Error: "OpenGL not found"
Pastikan driver graphics card sudah terinstall dengan benar.

---

## 🎮 Kontrol Program

| Tombol | Fungsi |
|--------|--------|
| W/A/S/D | Gerak maju/kiri/mundur/kanan |
| Mouse | Lihat sekeliling |
| Space | Naik |
| Shift | Turun |
| ESC | Keluar |
