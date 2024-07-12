#include "sv_subs_factory.h"
#include "main.h"

std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> SVSubscribe::get_sv_list()
{
    delete_sv_streams();
    
    SVSearchThread *thread = new SVSearchThread;
    if (thread->Create() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't create thread!" << std::endl;
        return nullptr;
    }
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
    wxGetApp().m_threads.Add(thread);
    if (thread->Run() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't start thread!" << std::endl;
        return nullptr;
    }

    return sv_list;
}

void SVSubscribe::delete_sv_streams()
{
    sv_list->clear();
    sv_list_raw->clear();
    sv_list_cnt->clear();
    sv_list_prev_time->clear();
    selectedSV_ids->clear();

}

void SVSubscribe::select_sv_streams()
{
    if (selectedSV_ids->empty())
    {
        std::cerr << "SV Stream is not selected!" << std::endl;
        return;
    }

    std::ostringstream filter_stream;
    filter_stream << "(";

    for (size_t idx = 0; idx < selectedSV_ids->size(); ++idx)
    {
        auto id = selectedSV_ids->at(idx);
        auto it = sv_list->begin();
        std::advance(it, id);

        if (it != sv_list->end())
        {
            if (idx > 0)
            {
                filter_stream << " or ";
            }
            filter_stream << "(ether dst ";
            for (int j = 0; j < ETHER_ADDR_LEN; ++j)
            {
                filter_stream << std::hex << static_cast<int>(it->ether_dhost[j]);
                if (j < ETHER_ADDR_LEN - 1)
                {
                    filter_stream << ":";
                }
            }

            filter_stream << " and ether src ";
            for (int j = 0; j < ETHER_ADDR_LEN; ++j)
            {
                filter_stream << std::hex << static_cast<int>(it->ether_shost[j]);
                if (j < ETHER_ADDR_LEN - 1)
                {
                    filter_stream << ":";
                }
            }
            filter_stream << " and ether proto 0x88ba";
            filter_stream << ")";
        }
        else
        {
            std::cerr << "Index out of range for id " << id << std::endl;
        }
    }

    filter_stream << ")";

    std::string filter_exp_str = filter_stream.str();
    std::cout << "filter_exp: " << filter_exp_str << std::endl;
    filter_exp = std::make_shared<std::vector<char>>(filter_exp_str.begin(), filter_exp_str.end());
    filter_exp->push_back('\0');
}

u_int64_t SVSubscribe::get_closer_freq(double raw_F)
{
    std::vector<u_int64_t> possible_F = {4000, 4800, 9600, 12800, 14400};
    u_int64_t minDiff = raw_F - possible_F[0];
    size_t ind_of_F = 0;

    for (size_t i = 0; i < possible_F.size(); ++i)
    {
        if ((std::abs(raw_F - possible_F[i])) < minDiff)
        {
            minDiff = std::abs(raw_F - possible_F[i]);
            ind_of_F = i;
        }
    }
    if (minDiff > 100)
    {
        return -1;  // If minimal difference is higher than 200 return error
    }
    else
    {
        return possible_F[ind_of_F];
    }
}

long SVSubscribe::find_sv_id(const SV_stream &sv)
{
    for (size_t idx = 0; idx < selectedSV_ids->size(); ++idx)
    {
        auto id = selectedSV_ids->at(idx);
        auto it = sv_list->begin();
        std::advance(it, id);

        if (it != sv_list->end())
        {
            const SV_stream &selected_sv = *it;

            if (std::memcmp(selected_sv.ether_dhost, sv.ether_dhost, ETHER_ADDR_LEN) == 0 &&
                std::memcmp(selected_sv.ether_shost, sv.ether_shost, ETHER_ADDR_LEN) == 0 &&
                selected_sv.APPID == sv.APPID &&
                selected_sv.noASDU == sv.noASDU &&
                selected_sv.svID == sv.svID &&
                selected_sv.DatSet == sv.DatSet)
            {
                return static_cast<long>(selectedSV_ids->at(idx));
            }
        }
    }
    return -1;
}
