/*
 * ScopeX
 * Copyright (C) 2024 Tekvel R&D Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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