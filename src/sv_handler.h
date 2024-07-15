#ifndef SV_HANDLER_H
#define SV_HANDLER_H

#include <memory>
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>

#include <arpa/inet.h>

#define CURRENT_SCALE 1000
#define VOLTAGE_SCALE 100

struct PhsMeas
{
    int32_t secData;
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

class SVHandler
{
public:
    SVHandler(uint64_t max_smpCnt, uint16_t DatSet);
    ~SVHandler();

    void ProcessData();

    std::vector<std::pair<uint16_t, std::vector<uint32_t>>> SV_data_raw;
    std::vector<SV> SV_data;

private:
    uint16_t num_of_meas;
};

bool comparePairs(const std::pair<uint16_t, std::vector<uint32_t>>& a,
                  const std::pair<uint16_t, std::vector<uint32_t>>& b);

#endif