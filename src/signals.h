#ifndef SIGNALS_H
#define SIGNALS_H

#include <vector>
#include <cmath>
#include <stdint.h>

#define AMPLITUDE 200.0f
#define FREQUENCY (50.0f / 1.0f)
#define PHASESHIFT120 (2.0f * M_PI / 3.0f)
#define PHASESHIFTNEG120 (-2.0f * M_PI / 3.0f)

#define samplingRate 4000
#define dt (1.0f/samplingRate)

struct Utc {
    float sec;
    float nsec;
};

struct Ts {
    Utc utcTime;
    int32_t smpCnt;
};

struct Value {
    float secData;
    uint16_t q;
};

struct Instance {
    Value val;
    Ts timeStamp;
};

class Signals {
public:
    static Signals& getInstance();
    void addSignal(float amplitude = AMPLITUDE, float frequency = FREQUENCY, float phaseShift = 0.0f);
    std::vector<Instance>& getData(size_t patternIndex);
    size_t getSignalLength() const;

private:
    Signals();
    Signals(const Signals&) = delete;
    Signals& operator=(const Signals&) = delete;
    void generateSignalPoints(float amplitude, float frequency, float phaseShift);

    std::vector<std::vector<Instance>> data;
};

#endif // SIGNALS_H
