#include "signal_analyzer.h"
#include "main.h"

SignalAnalyzer::SignalAnalyzer()
{
    std::cout << "Signal Analyzer is created" << std::endl;
}

SignalAnalyzer::~SignalAnalyzer()
{
    min_val.clear();
    max_val.clear();
    amplitude.clear();
}

void SignalAnalyzer::CalculateParameters()
{
    auto it = wxGetApp().sv_sub.sv_list->begin();
    std::advance(it, *wxGetApp().sv_sub.selectedSV_id_main);

    if (it != wxGetApp().sv_sub.sv_list->end())
    {
        auto noMeas = it->DatSet;
        min_val.resize(noMeas);
        max_val.resize(noMeas);
        amplitude.resize(noMeas);

        for (auto i = 0; i != noMeas; ++i)
        {
            min_val[i] = std::numeric_limits<float>::max();
            max_val[i] = std::numeric_limits<float>::min();
        }

        auto sv_handler_ptr = wxGetApp().sv_handler.GetSVHandler(*wxGetApp().sv_sub.selectedSV_id_main);

        if (!sv_handler_ptr->SV_data.empty())
        {
            for (const auto &data : sv_handler_ptr->SV_data)
            {
                for (auto i = 0; i != noMeas; ++i)
                {
                    // Max value
                    if (data.PhsMeasList[i].secData > max_val[i]){
                        max_val[i] = data.PhsMeasList[i].secData;
                    }
                    // Min value
                    if (data.PhsMeasList[i].secData < min_val[i]){
                        min_val[i] = data.PhsMeasList[i].secData;
                    }
                }
                
            }
            for (auto i = 0; i != noMeas; ++i)
            {
                amplitude[i] = (max_val[i] - min_val[i])/2;
                // std::cout << amplitude[i] << " " << min_val[i] << " " << max_val[i] << std::endl;
            }
            current_amp = *std::max_element(amplitude.begin(), amplitude.begin() + amplitude.size()/2);
            voltage_amp = *std::max_element(amplitude.begin() + amplitude.size()/2, amplitude.end());
        }
        else
        {
            std::cerr << "SV data is empty!" << std::endl;
        }
    }
    else
    {
        std::cerr << "Can't find SV data!" << std::endl;
    }
}