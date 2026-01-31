# 🪟 PANDUAN INSTALASI DI WINDOWS

## 🚀 LANGKAH CEPAT (3 LANGKAH PASTI BERHASIL)

### ✅ LANGKAH 1: Install CMake & MinGW

**A. Install CMake:**
1. Download: https://cmake.org/download/
2. Pilih file `.msi` (Windows x64 Installer)
3. Saat install, **WAJIB** centang: `Add CMake to system PATH`
4. Klik Next sampai selesai

**B. Install MinGW (Pilih salah satu cara):**

**Cara Mudah - WinLibs (RECOMMENDED):**
1. Buka: https://winlibs.com/
2. Scroll ke **Downloads** > **Release versions** > **UCRT runtime**
3. Pilih: **Win64** > **without LLVM/Clang/LLD/LLDB** > Download ZIP
4. Ekstrak ZIP ke `C:\mingw64`
5. Tambahkan ke PATH:
   - Buka **Settings** > cari **Environment Variables**
   - Klik **Environment Variables**
   - Di **System variables**, pilih **Path** > **Edit**
   - Klik **New** > ketik: `C:\mingw64\bin`
   - OK > OK > OK
6. **Restart Command Prompt / PowerShell**

**Cara Alternatif - MSYS2:**
1. Download: https://www.msys2.org/
2. Install MSYS2
3. Buka **MSYS2 UCRT64** dari Start Menu
4. Jalankan: `pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake make`
5. Tambahkan `C:\msys64\ucrt64\bin` ke PATH (seperti cara WinLibs di atas)
6. **Restart Command Prompt / PowerShell**

**Verifikasi Instalasi (WAJIB CEK):**
```cmd
cmake --version
gcc --version
```
Jika kedua command menampilkan versi, instalasi berhasil!

---

### ✅ LANGKAH 2: Download GLFW (Otomatis)

Buka **Command Prompt** atau **PowerShell**, lalu:
```cmd
cd Desktop\grafikkomputer\RoomSimulation3D\windows
setup_glfw.bat
```
Tunggu sampai muncul **"SETUP GLFW BERHASIL!"**

---

### ✅ LANGKAH 3: Build & Run

**Build:**
```cmd
cd Desktop\grafikkomputer\RoomSimulation3D\windows
build_windows.bat
```
Tunggu sampai muncul **"BUILD BERHASIL!"**

**Run:**
```cmd
run_windows.bat
```

---

## ⚠️ TROUBLESHOOTING

### Error: "'cmake' is not recognized"
- Pastikan CMake terinstall dengan centang "Add to PATH"
- **Restart Command Prompt** setelah install CMake

### Error: "'gcc' is not recognized"
- Pastikan MinGW terinstall (WinLibs/MSYS2)
- Pastikan `C:\mingw64\bin` sudah ditambahkan ke System PATH
- **Restart Command Prompt** setelah menambah PATH

### Error: "GLFW tidak ditemukan"
- Jalankan `setup_glfw.bat` terlebih dahulu
- Pastikan koneksi internet aktif

### Error: "Download gagal" di setup_glfw.bat
- Cek koneksi internet
- Download manual dari: https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
- Ekstrak ke folder `lib\glfw\`

### Build gagal dengan error apapun
1. Hapus folder `build_win` (jika ada)
2. Jalankan `build_windows.bat` lagi

---

## 🎮 Kontrol Program

| Tombol | Fungsi |
|--------|--------|
| W/A/S/D | Gerak maju/kiri/mundur/kanan |
| Mouse | Lihat sekeliling |
| Space | Naik |
| Shift | Turun |
| ESC | Keluar |

---

## 📁 Struktur Folder Setelah Setup

```
RoomSimulation3D/
├── lib/
│   ├── glad/           ✅ Sudah ada
│   ├── glm/            ✅ Sudah ada
│   └── glfw/           📥 Dari setup_glfw.bat
│       ├── include/GLFW/
│       ├── lib-mingw-w64/
│       └── lib-vc2022/
├── shaders/            ✅ Sudah ada
├── src/                ✅ Sudah ada
├── windows/
│   ├── setup_glfw.bat      [1] Download GLFW
│   ├── build_windows.bat   [2] Build program
│   ├── build_windows_vs.bat    Build dengan Visual Studio
│   ├── run_windows.bat     [3] Jalankan program
│   └── PANDUAN_WINDOWS.md
├── build_win/          📦 Hasil build (otomatis dibuat)
└── CMakeLists.txt
```

---

## 📞 Bantuan

Jika masih ada masalah:
1. Screenshot error yang muncul
2. Pastikan sudah mengikuti semua langkah dengan benar
3. Coba restart komputer lalu ulangi dari Langkah 2
