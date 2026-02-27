#!/bin/bash
# Sovereign-1 Spectral Edition: Calibration & Verification Script
echo "[*] Initializing Phase-Snap Engine Calibration..."

# Generate a 15-second test frame with 15% noise floor
# This simulates a typical urban pass in Singapore
./sovereign-synth-linux-amd64 15 0.15 > calibration.raw

# Feed the raw audio into the decoder
echo "[*] Feeding data into sovereign-linux-amd64..."
cat calibration.raw | ./sovereign-linux-amd64

# Cleanup
rm calibration.raw
echo "[*] Calibration Finished. Check output.pgm for results."
