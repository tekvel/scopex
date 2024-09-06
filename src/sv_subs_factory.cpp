/*
 * ScopeX
 * Copyright (C) 2024 Tekvel R&D Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sv_subs_factory.h"
#include "main.h"

SVSubscribe::SVSubscribe() : timer1(this, wxID_EVT_TIMER_DONE_2SEC)
{
    sv_list = std::make_shared<std::unordered_set<SV_stream, SV_stream::SVHashFunction>>();
    sv_list_raw = std::make_shared<std::unordered_set<SV_stream, SV_stream::SVHashFunction>>();
    max_smpCnt = std::make_shared<std::map<uintptr_t, u_int32_t>>();
    // sv_list_prev_time = std::make_shared<std::map<uintptr_t, u_int64_t>>();
    selectedSV_ids = std::make_shared<std::vector<long>>();
    selectedSV_id_main = std::make_shared<long>();
    filter_exp = std::make_shared<std::vector<char>>();

    // Timer events
	Bind(wxEVT_TIMER, &SVSubscribe::OnTimer, this, wxID_EVT_TIMER_DONE_2SEC);	// EVT_TIMER_DONE
}

SVSubscribe::~SVSubscribe()
{
}

std::shared_ptr<std::unordered_set<SV_stream, SV_stream::SVHashFunction>> SVSubscribe::get_sv_list()
{
    delete_sv_streams();
    
    search_thread = new SVSearchThread;
    if (search_thread->Create() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't create thread!" << std::endl;
        return nullptr;
    }
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
    wxGetApp().m_threads.Add(search_thread);
    if (search_thread->Run() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't start thread!" << std::endl;
        return nullptr;
    }

    timer1.StartOnce(2000);  // start timer

    return sv_list;
}

void SVSubscribe::delete_sv_streams()
{
    sv_list->clear();
    sv_list_raw->clear();
    max_smpCnt->clear();
    selectedSV_ids->clear();

}

void SVSubscribe::create_bpf_filter()
{
    if (selectedSV_id_main == nullptr)
    {
        std::cerr << "SV Stream is not selected!" << std::endl;
        return;
    }

    std::ostringstream filter_stream;
    filter_stream << "(";

    auto it = sv_list->begin();
    std::advance(it, *selectedSV_id_main);

    if (it != sv_list->end())
    {
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
        std::cerr << "Can't find selected SV among all SV" << std::endl;
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
    if (minDiff > 10)
    {
        return -1;  // If minimal difference is higher than 200 return error
    }
    else
    {
        return possible_F[ind_of_F];
    }
}

bool SVSubscribe::find_sv(const SV_stream &sv)
{
    auto it = sv_list->begin();
    std::advance(it, *selectedSV_id_main);

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
            return true;
        }
    }
    return false;
}

void SVSubscribe::OnTimer(wxTimerEvent &event)
{
    std::cout << "\nTimer is worked\n" << std::endl;
    if (search_thread != nullptr && search_thread->IsRunning())
    {
        search_thread->Stop();
        search_thread = nullptr;
    }
}