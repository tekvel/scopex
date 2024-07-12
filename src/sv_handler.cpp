#include "sv_handler.h"
#include "main.h"

SVHandler::SVHandler(uint64_t max_smpCnt, uint8_t DatSet)
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

    for (size_t i = 0; i != SV_data_raw.size(); ++i)
    {
        frame_sv.smpCnt = SV_data_raw[i].first;
        frame_sv.utcTime.sec = 110;
        frame_sv.utcTime.usec = 234322;

        for (int j = 0; j != num_of_meas; ++j)
        {
            frame_sv.PhsMeasList[j].secData = SV_data_raw[i].second[j*2];
            // std::cout << std::hex << SV_data_raw[i].second[j*2] << std::endl;
            frame_sv.PhsMeasList[j].q = SV_data_raw[i].second[j*2-1];
        }
    }
    std::cout << "Data successfully processed" << std::endl;
}