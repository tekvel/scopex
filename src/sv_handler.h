#ifndef SV_HANDLER_H
#define SV_HANDLER_H

#include <memory>
#include <vector>
#include <cstdint>

#define CURRENT_SCALE 1000
#define VOLTAGE_SCALE 100

struct PhsMeas
{
    uint32_t secData;
    uint32_t q;
};

struct SV
{
    struct 
    {
        float sec;
        float usec;
    } utcTime;

    uint8_t smpCnt;

    std::vector<PhsMeas> PhsMeasList;

    SV (int n)
    {
        PhsMeasList.resize(n);
    }
};

class SVHandler
{
public:
    SVHandler(uint64_t max_smpCnt, uint8_t DatSet);
    ~SVHandler();

    void ProcessData();

    std::vector<std::pair<uint16_t, std::vector<uint8_t>>> SV_data_raw;
    std::vector<SV> SV_data;
};

#endif