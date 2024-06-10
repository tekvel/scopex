#ifndef SIGNALS_H
#define SIGNALS_H

#include <wx/wx.h>
#include <cmath>
#include <array>
#include <vector>

#define AMPLITUDE 200.0f
#define FREQUENCY (50.0f * M_PI / 100000.0f)
#define PHASESHIFT120 (2.0f * M_PI / 3.0f)
#define PHASESHIFTNEG120 (-2.0f * M_PI / 3.0f)

class Signals {
public:
    static Signals& getInstance();

    std::vector<std::array<float, 2>>& getData();
    void generateSignalPoints(int numPoints, float amplitude = AMPLITUDE, float frequency = FREQUENCY, float phaseShift = 0.0f);
    // void generateSignalPoints(int numPoints, float amplitude = AMPLITUDE, float frequency = FREQUENCY, float phaseShift = 0.0f);
    size_t getSignalLength() const;

private:
    Signals() {}
    Signals(const Signals&) = delete;
    Signals& operator=(const Signals&) = delete;

    std::vector<std::array<float, 2>> data;
};

#endif
