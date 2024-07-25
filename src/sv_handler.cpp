#include "sv_handler.h"
#include "main.h"

SVHandler::SVHandler(uint16_t max_smpCnt, uint16_t DatSet)
    : num_of_meas(DatSet), num_of_points(max_smpCnt)
{
    InitializeAttributes();
    std::cout << max_smpCnt << ", " << DatSet*8 << std::endl;
}

SVHandler::~SVHandler()
{
    SV_data.clear();
    SV_data_raw.clear();
}

void SVHandler::InitializeAttributes()
{
    SV_data.clear();
    // Fill with zeros and resize SV_data
    SV frame_sv = SV(num_of_meas);
    frame_sv.smpCnt = 0;
    frame_sv.utcTime.sec = 0;
    frame_sv.utcTime.usec = 0;
    for (auto & data : frame_sv.PhsMeasList)
    {
        data.q = 0;
        data.secData = 0;
    }

    for (size_t i = 0; i != num_of_points; ++i)
    {
        SV_data.push_back(frame_sv);
    }

    // Resize SV_data_raw
    int i = 0;
    SV_data_raw.resize(2);
    for (auto &vectorPair : SV_data_raw) {
        vectorPair.resize(num_of_points);
        for (auto &pair : vectorPair) {
            pair.first = i;
            i += 1;
            pair.second.resize(num_of_meas * 2);
        }
    }

    operating_list = 0;

    reference_ts.first = 0;
    reference_ts.second = 0;
    reference_smpCnt = 0;
    prev_smpCnt = 0;
}

void SVHandler::ProcessData()
{
    SV frame_sv = SV(num_of_meas);
    
    // Clear data buffer
    if (!SV_data.empty())
    {
        for (auto &frame : SV_data)
        {
            frame.smpCnt = 0;
            frame.utcTime.sec = 0;
            frame.utcTime.sec = 0;

            for (auto & data : frame.PhsMeasList)
            {
                data.q = 0;
                data.secData = 0;
            }
        }
    }

    // Create a reference to non-operating list
    std::vector<std::pair<uint16_t, std::vector<uint32_t>>> &data_raw = SV_data_raw[operating_list == 0 ? 1 : 0];

    // Calculate local time from reference header time
    struct tm *time;
    time = gmtime(&reference_ts.first);

    float dt = 1.0/num_of_points;

    // Update SV_data
    for (size_t i = 0; i != data_raw.size(); ++i)
    {
        frame_sv.smpCnt = data_raw[i].first;
        frame_sv.utcTime.sec = time->tm_sec;
        frame_sv.utcTime.usec = dt * i;

        if (num_of_meas > 4)
        {
            for (auto j = 0; j != num_of_meas/2; ++j)
            {
                frame_sv.PhsMeasList[j].secData = static_cast<int32_t>(ntohl(data_raw[i].second[j*2]))/CURRENT_SCALE;
                frame_sv.PhsMeasList[j].q = data_raw[i].second[j*2-1];
            }
            for (auto j = num_of_meas/2; j != num_of_meas; ++j)
            {
                frame_sv.PhsMeasList[j].secData = static_cast<int32_t>(ntohl(data_raw[i].second[j*2]))/VOLTAGE_SCALE;
                frame_sv.PhsMeasList[j].q = data_raw[i].second[j*2-1];
            }
        }
        else
        {
            for (auto j = 0; j != num_of_meas; ++j)
            {
                frame_sv.PhsMeasList[j].secData = static_cast<int32_t>(ntohl(data_raw[i].second[j*2]))/CURRENT_SCALE;
                frame_sv.PhsMeasList[j].q = data_raw[i].second[j*2-1];
            }
        }
        
        SV_data[i] = frame_sv;
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