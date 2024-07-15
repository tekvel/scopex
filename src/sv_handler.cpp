#include "sv_handler.h"
#include "main.h"

SVHandler::SVHandler(uint64_t max_smpCnt, uint16_t DatSet)
    // : SV_data_raw(max_smpCnt, std::make_pair(0, std::vector<uint8_t>(DatSet*8)))
    : num_of_meas(DatSet)
{
    std::cout << max_smpCnt << ", " << DatSet*8 << std::endl;
}

SVHandler::~SVHandler()
{
    SV_data.clear();
    SV_data_raw.clear();
}

void SVHandler::ProcessData()
{
    SV frame_sv = SV(num_of_meas);
    
    SV_data.clear();    // Clear data buffer

    std::sort(SV_data_raw.begin(), SV_data_raw.end(), comparePairs);

    for (size_t i = 0; i != SV_data_raw.size(); ++i)
    {
        frame_sv.smpCnt = SV_data_raw[i].first;
        frame_sv.utcTime.sec = 110;
        frame_sv.utcTime.usec = 234322;

        for (int j = 0; j != num_of_meas; ++j)
        {
            frame_sv.PhsMeasList[j].secData = static_cast<int32_t>(ntohl(SV_data_raw[i].second[j*2]))/CURRENT_SCALE;
            // std::cout << std::hex << SV_data_raw[i].second[j*2] << std::endl;
            frame_sv.PhsMeasList[j].q = SV_data_raw[i].second[j*2-1];
        }
        SV_data.push_back(frame_sv);
    }
    std::cout << "Data successfully processed" << std::endl;

    SV_data_raw.clear();    // Clear raw_data buffer
}

bool comparePairs(const std::pair<uint16_t, std::vector<uint32_t>>& a,
                  const std::pair<uint16_t, std::vector<uint32_t>>& b)
{
    return a.first < b.first;
}