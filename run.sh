#!/bin/bash

echo "=========================================="
echo "  RUN SCRIPT - Simulasi Ruangan 3D"
echo "=========================================="

if [ -f "build/RoomSimulation3D" ]; then
    cd build
    ./RoomSimulation3D
else
    echo "Executable tidak ditemukan!"
    echo "Jalankan build.sh terlebih dahulu."
fi
