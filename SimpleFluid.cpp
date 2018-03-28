#include <iostream>
#include <cstdio>
#include <array>
#include <cmath>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>

std::size_t const kBufferSize = 80;
char const * kGrayScaleTable = " .:-=+*#%@";
std::size_t const kGrayScaleTableSize = sizeof(kGrayScaleTable)/sizeof(char);

void
accumulateWaveToHeightField(double x, double waveLength, double maxHeight, std::array<double, kBufferSize> & heightField) {
    double quarterWaveLength = 0.25 * waveLength;

    int start = static_cast<int>((x-quarterWaveLength) * kBufferSize);
    int end   = static_cast<int>((x+quarterWaveLength) * kBufferSize);

    for (int i = start; i < end; ++i) {
        int iNew = i;
        if (i < 0) iNew = -i - 1;
        else if ( i >= static_cast<int>(kBufferSize)) iNew = 2 * kBufferSize - i - 1;
        
        double distance = std::fabs((i + 0.5) / kBufferSize - x );
        double height   = maxHeight * 0.5 * (std::cos(std::min(distance * M_PI / quarterWaveLength, M_PI)) + 1.0);
        heightField[iNew] += height;
    }
}
void
updateWave(double timeInterval, double & x, double & speed) {
    if (x > 1.0) speed *= -1.0;
    else if (x < 0.0)  speed *= -1.0;
    x += timeInterval * speed; 
}

void
draw(std::array<double, kBufferSize> const & heightField) {
    std::string buffer(kBufferSize, ' ');

    for (std::size_t i = 0; i < kBufferSize; ++i) {
        double height = heightField[i];
        std::size_t tableIndex = std::min( static_cast<std::size_t>(std::floor(kGrayScaleTableSize * height)), kGrayScaleTableSize - 1);
        buffer[i] = kGrayScaleTable[tableIndex];
    }

    for (std::size_t i = 0; i < kBufferSize; ++i) {
        printf("\b");
    }
    printf("%s", buffer.c_str());
    fflush(stdout);
}
int main() {
    double x = 0.0;
    double y = 1.0f;
    double speedX = 1.0;
    double speedY = -0.5;

    double maxHeightX = 0.5;
    double maxHeightY = 0.4;

    double waveLengthX = 0.8;
    double waveLengthY = 1.2;

    int fps = 100;
    double timeInterval = 1.0 / fps;
    std::array<double, kBufferSize> heightField;

    for (int i = 0; i < 1000; ++i) {
        updateWave(timeInterval, x, speedX);
        updateWave(timeInterval, y, speedY);
        for (auto & height : heightField) {
            height = 0.0;
        }
        accumulateWaveToHeightField(x, waveLengthX, maxHeightX, heightField);
        accumulateWaveToHeightField(y, waveLengthY, maxHeightY, heightField);

        draw(heightField);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
    }
    printf("\n");
    fflush(stdout);
}