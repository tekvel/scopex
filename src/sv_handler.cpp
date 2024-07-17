#include "sv_handler.h"
#include "main.h"

SVHandler::SVHandler(uint16_t max_smpCnt, uint16_t DatSet)
    : num_of_meas(DatSet)
{
    // SV_data_raw.resize(2);
    // for (auto& innerVec : SV_data_raw) {
    //     innerVec.resize(max_smpCnt, SV_raw(DatSet));
    // }
    int i = 0;
    SV_data_raw.resize(2);
    for (auto &vectorPair : SV_data_raw) {
        vectorPair.resize(max_smpCnt);
        for (auto &pair : vectorPair) {
            pair.first = i;
            i += 1;
            pair.second.resize(DatSet * 2);
        }
    }

    operating_list = 0;

    reference_ts.first = 0;
    reference_ts.second = 0;
    reference_smpCnt = -1;
    prev_smpCnt = 0;

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

    std::vector<std::pair<uint16_t, std::vector<uint32_t>>> &data_raw = SV_data_raw[operating_list == 0 ? 1 : 0];

    for (size_t i = 0; i != data_raw.size(); ++i)
    {
        frame_sv.smpCnt = data_raw[i].first;
        frame_sv.utcTime.sec = 110;
        frame_sv.utcTime.usec = 234322;

        for (int j = 0; j != num_of_meas; ++j)
        {
            frame_sv.PhsMeasList[j].secData = static_cast<int32_t>(ntohl(data_raw[i].second[j*2]))/CURRENT_SCALE;
            frame_sv.PhsMeasList[j].q = data_raw[i].second[j*2-1];
        }
        SV_data.push_back(frame_sv);
    }
    std::cout << "Data successfully processed" << std::endl;

    // Clear raw data buffer
    int i = 0;
    for (auto &pair : SV_data_raw[operating_list == 0 ? 1 : 0])
    {
        pair.first = i;
        i += 1;
        for (auto & val :pair.second)
        {
            val = 0;
        }
    }

}

bool comparePairs(const std::pair<uint16_t, std::vector<uint32_t>>& a,
                  const std::pair<uint16_t, std::vector<uint32_t>>& b)
{
    return a.first < b.first;
}