#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>

int main() {
    const int samples_per_line = 22050;
    const int out_width = 2080;
    const int chan_width = 909;
    const int chB_start = 1125;

    std::vector<int16_t> line_buffer(samples_per_line);
    std::vector<uint8_t> imgB;
    int line_count = 0;

    while (std::cin.read(reinterpret_cast<char*>(line_buffer.data()), samples_per_line * 2)) {
        std::vector<float> env(samples_per_line);
        float max_v = 1.0f;
        float current_energy = 0;

        for(int i=0; i<samples_per_line; ++i) {
            float s = (float)line_buffer[i];
            env[i] = std::abs(s);
            if(env[i] > max_v) max_v = env[i];
            current_energy += env[i];
        }

        // --- CTT PHASE SNAP LOGIC ---
        // We calculate SNR by comparing sync-pulse energy to line noise
        float snr_value = (max_v / (current_energy / samples_per_line)) * 2.0f;
        bool sync_locked = (snr_value > 4.5); // Threshold for a clean "Snap"

        if (sync_locked) {
            std::cerr << "\r[ PHASE SNAP LOCKED ] SNR: " << snr_value << " dB    " << std::flush;
            // Write telemetry for Web GUI
            std::ofstream status("data/current_status.json");
            status << "{\"locked\": true, \"snr\": " << snr_value << ", \"line\": " << line_count << "}";
            status.close();
        } else {
            std::cerr << "\r[ SEARCHING FOR SYNC... ]                " << std::flush;
        }

        for (int x = 0; x < out_width; x++) {
            int idx = x * (samples_per_line / (float)out_width);
            uint8_t pixel = (uint8_t)std::clamp((env[idx] / max_v) * 255.0f, 0.0f, 255.0f);
            if (x >= chB_start && x < chB_start + chan_width) imgB.push_back(pixel);
        }
        line_count++;
    }

    // Save final Product
    int height = imgB.size() / chan_width;
    std::ofstream fC("channel_B_filtered.ppm");
    fC << "P6\n" << chan_width << " " << height << "\n255\n";
    for (uint8_t p : imgB) {
        // CTT Thermal Mapping
        uint8_t r = (p > 150) ? 255 : (p < 50) ? 0 : p;
        uint8_t g = (p > 100) ? p : 50;
        uint8_t b = (p < 100) ? 255 : p;
        fC.put(r); fC.put(g); fC.put(b);
    }
    return 0;
}
