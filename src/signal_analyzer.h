#ifndef SIGNAL_ANALYZER_H
#define SIGNAL_ANALYZER_H

#include <vector>
#include <limits>
#include <algorithm>

class SignalAnalyzer
{
public:
    SignalAnalyzer();
    ~SignalAnalyzer();

    std::vector<float> min_val;
    std::vector<float> max_val;
    std::vector<float> amplitude;

    float voltage_amp;
    float current_amp;

    void CalculateParameters();

private:
};

#endif