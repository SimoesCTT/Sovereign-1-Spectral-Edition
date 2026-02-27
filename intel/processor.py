#!/usr/bin/env python3
import numpy as np
import sys
import json
from scipy import signal

# CTT Specs
SR = 44100
FFT_SIZE = 2048

def process_stream():
    print("⚡ CTT Spectral Engine: Active", file=sys.stderr)
    while True:
        raw_bits = sys.stdin.buffer.read(SR * 2)
        if not raw_bits: break
        
        audio = np.frombuffer(raw_bits, dtype=np.int16).astype(np.float32)
        f, t, Zxx = signal.stft(audio, fs=SR, nperseg=FFT_SIZE)
        
        # --- SPECTRAL EXPORT FOR GUI ---
        # Capture the magnitude around the 2400Hz carrier
        mag = np.abs(Zxx).mean(axis=1)
        # Narrow the view to 1kHz - 4kHz for the GUI
        waterfall_slice = mag[(f > 1000) & (f < 4000)].tolist()
        
        with open("data/waterfall.json", "w") as j:
            json.dump(waterfall_slice, j)

        # Signal Cleaning (Your CTT Physics)
        mask = (f > 2300) & (f < 2500)
        Zxx_filtered = np.zeros_like(Zxx)
        Zxx_filtered[mask, :] = Zxx[mask, :]
        
        _, clean_audio = signal.istft(Zxx_filtered, fs=SR)
        sys.stdout.buffer.write(clean_audio.astype(np.int16).tobytes())

if __name__ == "__main__":
    process_stream()
