#!/bin/bash
echo "🛠️  Packaging Sovereign-1 (Black Edition)..."

# Install system-level libs
sudo dnf install -y gcc-c++ python3-pip python3-scipy python3-numpy rtl-sdr ImageMagick

# Compile the C++ Imaging Engine
g++ -O3 ../sovereign_core.cpp -o bin/sovereign

# Setup the Web GUI
cp ../web/index.html web/
touch data/current_status.json

echo "✅ Package Integrated. Ready for Mission Start."
