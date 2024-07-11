#include "sv_handler.h"
#include "main.h"

SVHandler::SVHandler(uint64_t max_smpCnt, uint8_t DatSet)
    : SV_data_raw(max_smpCnt, std::make_pair(0, std::vector<uint8_t>(DatSet*8)))
{
}

SVHandler::~SVHandler()
{
    SV_data.clear();
    SV_data_raw.clear();
}

void SVHandler::ProcessData()
{

}