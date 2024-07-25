#include "sv_stream_thread.h"
#include "main.h"

SVSearchThread::SVSearchThread() : wxThread(wxTHREAD_DETACHED)
{
}

SVSearchThread::~SVSearchThread()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);

    // wxGetApp().network_interface.stop_capture();

    wxArrayThread &threads = wxGetApp().m_threads;
    threads.Remove(this);

    if (threads.IsEmpty())
    {
        if (wxGetApp().m_shuttingDown)
        {
            wxGetApp().m_shuttingDown = false;

            wxGetApp().m_semAllDone.Post();
        }
    }
}

wxThread::ExitCode SVSearchThread::Entry()
{
    SetName("SV Searching Thread");

    std::cout << "SV Search Thread started" << std::endl;

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }
        
    char filter_exp[] = "ether proto 0x88ba";   // Ethernet protocol of SV (IEC 61850)
    int error = wxGetApp().network_interface.start_capture(filter_exp, "parse_sv_streams");
    std::cerr << "Error in start_capture: " << error << std::endl;

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }

    for (const auto &sv_raw : *wxGetApp().sv_sub.sv_list_raw)
    {
        SV_stream stream;
        std::copy(std::begin(sv_raw.ether_dhost), std::end(sv_raw.ether_dhost), std::begin(stream.ether_dhost));
        std::copy(std::begin(sv_raw.ether_shost), std::end(sv_raw.ether_shost), std::begin(stream.ether_shost));
        stream.APPID = sv_raw.APPID;
        stream.svID = sv_raw.svID;
        stream.noASDU = sv_raw.noASDU;
        stream.F = sv_raw.F;
        stream.DatSet = sv_raw.DatSet;

        for (auto &pair : *wxGetApp().sv_sub.max_smpCnt)
        {
            if (reinterpret_cast<uintptr_t>(&sv_raw) == pair.first)
            {
                u_int64_t F = wxGetApp().sv_sub.get_closer_freq(pair.second);
                if (F != -1)
                {
                    stream.F = F;
                    wxGetApp().sv_sub.sv_list->insert(stream);
                }
            }
        }
    }
    // Post "SV Search Completed" event to SV_dialog window
    if (wxGetApp().GetMainFrame()->SV_dialog != nullptr && !wxGetApp().GetMainFrame()->SV_dialog->IsBeingDeleted())
    {
        wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_SEARCH_COMPLETED);
        wxQueueEvent(wxGetApp().GetMainFrame()->SV_dialog, event);
    }

    return (wxThread::ExitCode)NULL;
}

void SVSearchThread::Stop()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);
    if (wxGetApp().network_interface.isCapturing)
    {
        wxGetApp().network_interface.stop_capture();
        wxGetApp().sv_sub.search_thread = nullptr;
    }
}