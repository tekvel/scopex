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
    bool tagged;
    u_int16_t ether_type;

    bool operator==(const SV_stream &other) const
    {
        if (std::memcmp(ether_dhost, other.ether_dhost, ETHER_ADDR_LEN) == 0 &&
            std::memcmp(ether_shost, other.ether_shost, ETHER_ADDR_LEN) == 0 &&
            tagged == other.tagged &&
            ether_type == other.ether_type)
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
            std::size_t h3 = std::hash<bool>{}(sv.tagged);
            std::size_t h4 = std::hash<u_int16_t>{}(sv.ether_type);

            return (h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3));
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
