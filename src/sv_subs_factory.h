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

#ifndef SV_SUBS_FACTORY_H
#define SV_SUBS_FACTORY_H

#include <pcap.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>
#include <functional>
#include <unordered_set>
#include <map>
#include <cstring>
#include <sstream>

#include <wx/timer.h>

#include "sv_stream_thread.h"

#define ETHER_ADDR_LEN 6

struct SV_stream
{
    u_int8_t ether_dhost[ETHER_ADDR_LEN];
    u_int8_t ether_shost[ETHER_ADDR_LEN];
    u_int16_t APPID;
    std::vector<u_char> svID;
    u_char noASDU;
    u_int64_t F;
    u_char DatSet;

    bool operator==(const SV_stream &other) const
    {
        if (std::memcmp(ether_dhost, other.ether_dhost, ETHER_ADDR_LEN) == 0 &&
            std::memcmp(ether_shost, other.ether_shost, ETHER_ADDR_LEN) == 0 &&
            APPID == other.APPID &&
            svID == other.svID &&
            noASDU == other.noASDU &&
            F == other.F &&
            DatSet == other.DatSet)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    struct SVHashFunction
    {
        std::size_t operator()(const SV_stream &sv) const
        {
            std::size_t h1 = std::hash<std::string>{}(std::string(reinterpret_cast<const char *>(sv.ether_dhost), ETHER_ADDR_LEN));
            std::size_t h2 = std::hash<std::string>{}(std::string(reinterpret_cast<const char *>(sv.ether_shost), ETHER_ADDR_LEN));
            std::size_t h3 = std::hash<u_int16_t>{}(sv.APPID);
            std::size_t h4 = 0;
            for (u_char val : sv.svID)
            {
                h4 ^= std::hash<u_char>{}(val) + 0x9e3779b9 + (h4 << 6) + (h4 >> 2);
            }
            std::size_t h5 = std::hash<u_char>{}(sv.noASDU);
            std::size_t h6 = std::hash<u_int64_t>{}(sv.F);
            std::size_t h7 = std::hash<u_char>{}(sv.DatSet);

            return (h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6));
        }
    };
};

class SVSearchThread;

class SVSubscribe : public wxEvtHandler
{
public:
    SVSubscribe();
    ~SVSubscribe();

    std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> get_sv_list();
    void create_bpf_filter();
    void delete_sv_streams();
    u_int64_t get_closer_freq(double raw_F);
    bool find_sv(const SV_stream &sv);
    
    void OnTimer(wxTimerEvent &event);

    std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> sv_list_raw;
    std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> sv_list;

    std::shared_ptr<std::map<uintptr_t, u_int32_t>> max_smpCnt;

    std::shared_ptr<std::vector<long>> selectedSV_ids;
    std::shared_ptr<long> selectedSV_id_main;
    std::shared_ptr<std::vector<char>> filter_exp;

    SVSearchThread *search_thread;

private:
    wxTimer timer1;
};

enum
{
    wxID_EVT_TIMER_DONE_2SEC = 501,
};

#endif
