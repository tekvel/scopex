#include "signals.h"
#include <cmath>

Signals::Signals() {}

Signals& Signals::getInstance() {
    static Signals instance;
    return instance;
}

void Signals::addSignal(float amplitude, float frequency, float phaseShift) {
    generateSignalPoints(amplitude, frequency, phaseShift);
}

std::vector<Instance>& Signals::getData(size_t patternIndex) {
    return data.at(patternIndex);
}

size_t Signals::getSignalLength() const {
    return data.empty() ? 0 : data.front().size();
}

void Signals::generateSignalPoints(float amplitude, float frequency, float phaseShift) {
    std::vector<Instance> sig(16000);
    for (size_t i = 0; i < sig.size(); ++i) {
        float time = static_cast<float>(i);
        sig[i].val.secData = amplitude * sin(frequency * time + phaseShift);
        sig[i].val.q = 0xFFFF;
        sig[i].timeStamp.utcTime.sec = static_cast<int32_t>(time);
        sig[i].timeStamp.utcTime.nsec = static_cast<int32_t>(time * 1e9);
        sig[i].timeStamp.smpCnt = static_cast<int32_t>(i);
    }
    data.push_back(sig);
}
