#ifndef SV_SUBS_FACTORY_H
#define SV_SUBS_FACTORY_H

#include <pcap.h>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include "sv_stream_thread.h"

class SVSubscribe
{
public:
    SVSubscribe() : sv_list(std::make_shared<std::vector<std::string>>()) {}
    ~SVSubscribe() {}

    std::shared_ptr<std::vector<std::string>> get_sv_list();
    void select_sv_streams(std::vector<int> ids);

private:
    std::shared_ptr<std::vector<std::string>> sv_list;
};

#endif
