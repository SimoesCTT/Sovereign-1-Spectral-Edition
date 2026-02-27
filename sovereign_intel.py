import numpy as np
from scipy import signal
import sys

# CTT Constants from your script
PHI = (1 + np.sqrt(5)) / 2
RIEMANN_ZEROS = np.array([14.134725, 21.022040, 25.010858, 30.424876])

def sovereign_filter(audio_data, sr=44100):
    # Perform STFT (Short-Time Fourier Transform)
    f, t, Zxx = signal.stft(audio_data, fs=sr, nperseg=2048)
    
    # CTT Logic: Find the 2400Hz carrier spike
    carrier_idx = np.argmin(np.abs(f - 2400))
    
    # Noise Gate: Use your noise_floor logic to mask everything 
    # except the satellite subcarrier
    magnitude = np.abs(Zxx)
    noise_floor = np.mean(magnitude) * 1.5
    Zxx[magnitude < noise_floor] = 0
    
    # Reconstruct clean audio
    _, clean_audio = signal.istft(Zxx, fs=sr)
    return clean_audio.astype(np.int16)

# Pipe Processing
if __name__ == "__main__":
    sr = 44100
    chunk_size = 44100 # 1 second of audio
    while True:
        raw_data = sys.stdin.buffer.read(chunk_size * 2)
        if not raw_data: break
        
        audio = np.frombuffer(raw_data, dtype=np.int16)
        processed = sovereign_filter(audio, sr)
        sys.stdout.buffer.write(processed.tobytes())
