#include <iostream>
#include <cmath>
#include <cstdint>
#include <random>

int main() {
    const int sample_rate = 44100;
    const float duration = 30.0f;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-2000.0, 2000.0);

    for (int i = 0; i < (int)(sample_rate * duration); ++i) {
        float time = (float)i / sample_rate;
        float subcarrier = std::sin(2.0f * M_PI * 2400.0f * time);
        float line_pos = std::fmod(time, 0.5f) / 0.5f;
        float signal = (line_pos < 0.05f) ? 0.9f : 0.4f;

        // Add heavy random static noise
        float noise = distribution(generator);
        int16_t sample = static_cast<int16_t>(signal * subcarrier * 16000.0f + noise);
        
        std::cout.write(reinterpret_cast<char*>(&sample), 2);
    }
    return 0;
}
