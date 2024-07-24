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

/*wxThread::ExitCode SVSearchThread::Entry()
{
    SetName("SV Searching Thread");

    std::cout << "SV Search Thread started" << std::endl;

    int k = 10, num_packets = 192;

    for (int i = 0; i != k; ++i)
    {
        {
            wxCriticalSectionLocker locker(wxGetApp().m_critsect);
            if (wxGetApp().m_shuttingDown)
                return NULL;
        }
        if (TestDestroy())
            break;

        char filter_exp[] = "ether proto 0x88ba";   // Ethernet protocol of SV (IEC 61850)
        wxGetApp().network_interface.sniff_traffic(num_packets, filter_exp, "parse_sv_streams", 100);
        
        wxThread::Sleep(10);
    }

    for (auto& pair : *wxGetApp().sv_sub.sv_list_cnt) {
        pair.second = 0;    // Reset counter of SV streams
    }

    // wxThread::Sleep(1000);
    std::cout << "\nHello from thread" << std::endl;

    wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_SEARCH_COMPLETED);
    wxQueueEvent(wxGetApp().GetMainFrame()->SV_dialog, event);

    std::cout << "Number of streams: " << wxGetApp().sv_sub.sv_list->size() << std::endl;

    return (wxThread::ExitCode)NULL;
}*/

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
    int error = wxGetApp().network_interface.start_capture(filter_exp, "got_packet");
    std::cerr << "Error in start_capture: " << error << std::endl;

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }

    // wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_SEARCH_COMPLETED);
    // wxQueueEvent(wxGetApp().GetMainFrame()->SV_dialog, event);

    // std::cout << "Number of streams: " << wxGetApp().sv_sub.sv_list->size() << std::endl;

    return (wxThread::ExitCode)NULL;
}

void SVSearchThread::Stop()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);
    wxGetApp().network_interface.stop_capture();
}