#include "signals.h"

Signals& Signals::getInstance() {
    static Signals instance;
    return instance;
}

std::vector<std::array<float, 2>>& Signals::getData() {
    return data;
}

void Signals::generateSignalPoints(int numPoints, float amplitude, float frequency, float phaseShift) {
    data.resize(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        data[i] = { static_cast<float>(i), amplitude * std::sin(frequency * i + phaseShift) };
    }
}

size_t Signals::getSignalLength() const {
    return data.size();
}
