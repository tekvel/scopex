#include "sv_subs_factory.h"
#include "main.h"

std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> SVSubscribe::get_sv_list()
{
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
}

void SVSubscribe::select_sv_streams(std::vector<long> *ids)
{
    if (ids->empty())
    {
        std::cerr << "SV Stream is not selected!" << std::endl;
        return;
    }

    std::ostringstream filter_stream;
    filter_stream << "(";

    for (size_t idx = 0; idx < ids->size(); ++idx)
    {
        auto id = ids->at(idx);
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

            filter_stream << " and ether proto 0x" << std::hex << it->ether_type;
            filter_stream << ")";
        }
        else
        {
            std::cerr << "Index out of range for id " << id << std::endl;
        }
    }

    filter_stream << ")";

    std::string filter_exp = filter_stream.str();
    std::cout << "filter_exp: " << filter_exp << std::endl;
    std::vector<char> filter_cstr(filter_exp.begin(), filter_exp.end());
    filter_cstr.push_back('\0');

    wxGetApp().network_interface.sniff_traffic(2, filter_exp.data(), "got_packet", 100);
}