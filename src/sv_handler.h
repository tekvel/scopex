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

#ifndef SV_HANDLER_H
#define SV_HANDLER_H

#include <memory>
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <ctime>

#include <arpa/inet.h>

#include "signal_analyzer.h"

#define CURRENT_SCALE 1000.0
#define VOLTAGE_SCALE 100.0

struct PhsMeas
{
    float secData;
    uint32_t q;
};

struct SV
{
    struct 
    {
        float sec;
        float usec;
    } utcTime;

    uint16_t smpCnt;

    std::vector<PhsMeas> PhsMeasList;

    SV (int n)
    {
        PhsMeasList.resize(n);
    }
};

struct SV_raw
{
    uint16_t smpCnt;
    
    std::vector<PhsMeas> PhsMeasList;

    SV_raw (int n)
    {
        PhsMeasList.resize(n);
    }
};

class SVHandler
{
public:
    SVHandler(uint16_t max_smpCnt, uint16_t DatSet);
    ~SVHandler();

    void InitializeAttributes();
    void ProcessData();

    std::vector<std::vector<std::pair<uint16_t, std::vector<uint32_t>>>> SV_data_raw;
    std::vector<SV> SV_data;

    uint16_t prev_smpCnt;
    uint32_t operating_list;
    std::pair<long int, long int> reference_ts;
    uint16_t reference_smpCnt;
    // std::vector<std::vector<SV_raw>> SV_data_raw;

    SignalAnalyzer SAnalyzer;

private:
    uint16_t num_of_meas;
    uint16_t num_of_points;
};

#endif