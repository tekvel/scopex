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

private:
    uint16_t num_of_meas;
    uint16_t num_of_points;
};

#endif