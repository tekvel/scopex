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
#include <cstring>
#include <sstream>

#include "sv_stream_thread.h"

#define ETHER_ADDR_LEN 6

struct SV_stream
{
    u_int8_t ether_dhost[ETHER_ADDR_LEN];
    u_int8_t ether_shost[ETHER_ADDR_LEN];
    u_int16_t APPID;
    u_int16_t Length;
    std::vector<u_char> svID;
    u_char noASDU;
    u_char DatSet;

    bool operator==(const SV_stream &other) const
    {
        if (std::memcmp(ether_dhost, other.ether_dhost, ETHER_ADDR_LEN) == 0 &&
            std::memcmp(ether_shost, other.ether_shost, ETHER_ADDR_LEN) == 0 &&
            APPID == other.APPID &&
            Length == other.Length &&
            svID == other.svID &&
            noASDU == other.noASDU &&
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
            std::size_t h4 = std::hash<u_int16_t>{}(sv.Length);
            std::size_t h5 = 0;
            for (u_char val : sv.svID)
            {
                h5 ^= std::hash<u_char>{}(val) + 0x9e3779b9 + (h5 << 6) + (h5 >> 2);
            }
            std::size_t h6 = std::hash<u_char>{}(sv.noASDU);
            std::size_t h7 = std::hash<u_char>{}(sv.DatSet);

            return (h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4) ^ (h6 << 5) ^ (h7 << 6));
        }
    };
};

class SVSubscribe
{
public:
    SVSubscribe() : sv_list(std::make_shared<std::unordered_set<SV_stream, SV_stream::SVHashFunction>>()) {}
    ~SVSubscribe() {}

    std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> get_sv_list();
    void select_sv_streams(std::vector<long> *ids);
    void delete_sv_streams();

    std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> sv_list;

private:
};

#endif
