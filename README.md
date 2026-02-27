# 🛰️ SOVEREIGN-1: SPECTRAL EDITION
Equatorial Satellite Ground Station with CTT Phase-Snap & FFT Spectral Isolation.

## 🚀 Installation
1. Run `./install.sh` to compile the C++ core and install dependencies.
2. Open `web/index.html` in your browser.

## 📡 Live Deciphering
`rtl_fm -f 137.1M -s 44100 | ./intel/processor.py | ./bin/sovereign`
