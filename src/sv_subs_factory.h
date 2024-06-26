#ifndef SV_SUBS_FACTORY_H
#define SV_SUBS_FACTORY_H

#include <pcap.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>

#include "sv_stream_thread.h"

#define ETHER_ADDR_LEN 6

struct SV_stream
{
    u_int8_t ether_dhost[ETHER_ADDR_LEN];
    u_int8_t ether_shost[ETHER_ADDR_LEN];
    bool tagged;
    u_int16_t ether_type;
};

class SVSubscribe
{
public:
    SVSubscribe() : sv_list(std::make_shared<std::vector<SV_stream>>()) {}
    ~SVSubscribe() {}

    std::shared_ptr<std::vector<SV_stream>> get_sv_list();
    void select_sv_streams(std::vector<int> ids);

private:
    std::shared_ptr<std::vector<SV_stream>> sv_list;
};

#endif
