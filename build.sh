#!/bin/bash

echo "=========================================="
echo "  BUILD SCRIPT - Simulasi Ruangan 3D"
echo "=========================================="

# Cek apakah folder build sudah ada
if [ ! -d "build" ]; then
    echo "Membuat folder build..."
    mkdir build
fi

cd build

echo "Menjalankan CMake..."
cmake ..

echo "Kompilasi..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "  BUILD BERHASIL!"
    echo "=========================================="
    echo "Jalankan dengan: ./build/RoomSimulation3D"
    echo ""
else
    echo ""
    echo "=========================================="
    echo "  BUILD GAGAL!"
    echo "=========================================="
    echo "Periksa error di atas."
fi
