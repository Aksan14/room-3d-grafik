# =====================================================================
# DOKUMENTASI PROYEK
# SIMULASI RUANG KERJA 3D - GRAFIKA KOMPUTER
# =====================================================================

## 📁 Struktur Folder Proyek

```
RoomSimulation3D/
├── lib/
│   ├── glad/
│   │   ├── include/
│   │   │   ├── glad/
│   │   │   │   └── glad.h          ← Download dari glad.dav1d.de
│   │   │   └── KHR/
│   │   │       └── khrplatform.h   ← Download dari glad.dav1d.de
│   │   └── src/
│   │       └── glad.c              ← Download dari glad.dav1d.de
│   └── glm/
│       └── glm/                    ← Download dari github.com/g-truc/glm
│           ├── glm.hpp
│           ├── vec3.hpp
│           ├── mat4x4.hpp
│           └── ... (semua file glm)
├── shaders/
│   ├── vertex_shader.glsl
│   ├── fragment_shader.glsl
│   ├── light_vertex.glsl
│   └── light_fragment.glsl
├── src/
│   └── main.cpp
├── CMakeLists.txt
├── build.sh
├── run.sh
└── README.md
```

---

## 📥 LANGKAH DOWNLOAD & SETUP LIBRARY

### 1. GLAD (OpenGL Loader)
**Website:** https://glad.dav1d.de/

**Langkah:**
1. Buka https://glad.dav1d.de/
2. Set pengaturan:
   - Language: **C/C++**
   - Specification: **OpenGL**
   - API gl: **Version 3.3** (atau lebih tinggi)
   - Profile: **Core**
   - ✅ Centang **Generate a loader**
3. Klik tombol **GENERATE**
4. Download file **glad.zip**
5. Ekstrak dan pindahkan:
   - `include/glad/glad.h` → `lib/glad/include/glad/glad.h`
   - `include/KHR/khrplatform.h` → `lib/glad/include/KHR/khrplatform.h`
   - `src/glad.c` → `lib/glad/src/glad.c`

### 2. GLM (OpenGL Mathematics)
**Website:** https://github.com/g-truc/glm/releases

**Langkah:**
1. Download versi terbaru (contoh: glm-0.9.9.8.zip)
2. Ekstrak file zip
3. Copy folder `glm/glm/` ke `lib/glm/glm/`
   - Pastikan struktur: `lib/glm/glm/glm.hpp` ada

### 3. GLFW (Window & Input Library)
**Website:** https://www.glfw.org/download.html

**Untuk Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install libglfw3-dev libglfw3
```

**Untuk Arch Linux:**
```bash
sudo pacman -S glfw-x11
```

**Untuk Fedora:**
```bash
sudo dnf install glfw-devel
```

---

## 🔧 CARA BUILD & RUN

### Prasyarat
Pastikan sudah terinstall:
- GCC/G++ compiler
- CMake (minimal versi 3.10)
- OpenGL development libraries
- GLFW3

**Install di Ubuntu/Debian:**
```bash
sudo apt-get install build-essential cmake libgl1-mesa-dev libglfw3-dev
```

### Build Project
```bash
cd RoomSimulation3D
chmod +x build.sh
./build.sh
```

### Run Project
```bash
./run.sh
# atau langsung:
./build/RoomSimulation3D
```

---

## 🎮 KONTROL PROGRAM

| Tombol | Fungsi |
|--------|--------|
| W | Maju |
| S | Mundur |
| A | Kiri |
| D | Kanan |
| SPACE | Naik |
| SHIFT | Turun |
| Mouse | Rotasi pandangan |
| Scroll | Zoom in/out |
| 1 | View depan |
| 2 | View atas (top-down) |
| 3 | View samping |
| 4 | View sudut (isometric) |
| L | Toggle lampu ON/OFF |
| ESC | Keluar |

---

## 📐 PENJELASAN TEKNIS

### 1. Transformasi Geometris

**a. Translasi (Translation)**
```cpp
model = glm::translate(model, position);
```
Memindahkan objek dari posisi origin ke posisi yang diinginkan.

**b. Rotasi (Rotation)**
```cpp
model = glm::rotate(model, glm::radians(angle), axis);
```
Memutar objek pada sumbu tertentu (X, Y, atau Z).

**c. Skala (Scale)**
```cpp
model = glm::scale(model, scale);
```
Mengubah ukuran objek.

### 2. Sistem Koordinat
- **Model Space**: Koordinat lokal objek
- **World Space**: Koordinat dunia setelah model transformation
- **View Space**: Koordinat relatif terhadap kamera
- **Clip Space**: Koordinat setelah projection transformation

### 3. Pencahayaan Phong
Program menggunakan **Phong Lighting Model** dengan 3 komponen:

**a. Ambient Light**
```glsl
vec3 ambient = ambientStrength * lightColor;
```
Cahaya dasar yang merata di semua permukaan.

**b. Diffuse Light**
```glsl
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diffuseStrength * diff * lightColor;
```
Cahaya berdasarkan sudut permukaan terhadap sumber cahaya.

**c. Specular Light**
```glsl
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
vec3 specular = specularStrength * spec * lightColor;
```
Pantulan cahaya yang membuat efek mengkilap.

### 4. Kamera
Menggunakan **LookAt Camera** dengan:
- Position: Lokasi kamera di world space
- Front: Arah pandang kamera
- Up: Vektor atas kamera

```cpp
glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
```

### 5. Projection
Menggunakan **Perspective Projection**:
```cpp
glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, near, far);
```

---

## 🏠 OBJEK DALAM RUANGAN

1. **Lantai** - Pola kotak-kotak (checkered floor)
2. **Dinding** - 3 sisi dengan lubang jendela dan pintu
3. **Langit-langit** - Plafon putih
4. **Jendela** - Dengan frame kayu dan kaca
5. **Pintu** - Dengan handle dan panel dekoratif
6. **Meja Kerja** - Dengan laci
7. **Kursi** - Kursi kantor sederhana
8. **Monitor** - Layar komputer
9. **Keyboard** - Dengan tombol-tombol
10. **Lemari** - Lemari 2 pintu
11. **Lampu Gantung** - Sumber cahaya
12. **Buku** - Aksesori di meja
13. **Karpet** - Dengan border
14. **Meja Samping** - Dengan pot tanaman

---

## 🔍 ANALISIS KESULITAN

### Masalah yang Dihadapi:

1. **Normal Calculation**
   - Kesulitan: Menghitung normal yang benar untuk setiap permukaan
   - Solusi: Menggunakan face normals untuk setiap segitiga

2. **Lighting Artifacts**
   - Kesulitan: Shadow acne dan peter-panning
   - Solusi: Bias pada depth comparison

3. **Camera Gimbal Lock**
   - Kesulitan: Rotasi kamera terkunci pada sudut tertentu
   - Solusi: Membatasi pitch angle (-89° sampai +89°)

4. **Z-Fighting**
   - Kesulitan: Flickering pada permukaan yang berdekatan
   - Solusi: Menggunakan offset kecil pada posisi

5. **Memory Management**
   - Kesulitan: Memory leak dari OpenGL objects
   - Solusi: Proper cleanup dengan glDelete* functions

---

## 📚 REFERENSI

1. LearnOpenGL - https://learnopengl.com/
2. OpenGL Programming Guide
3. GLFW Documentation - https://www.glfw.org/docs/
4. GLM Documentation - https://glm.g-truc.net/

---

## 👨‍💻 DIBUAT UNTUK

**Mata Kuliah:** Grafika Komputer
**Proyek:** Simulasi Visual Ruangan 3D

---

*Dokumentasi ini dibuat untuk memenuhi tugas proyek grafika komputer*
