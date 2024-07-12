#include "sv_handler_thread.h"
#include "main.h"

SVHandlerThread::SVHandlerThread() : wxThread()
{
}

SVHandlerThread::~SVHandlerThread()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);

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

wxThread::ExitCode SVHandlerThread::Entry()
{
    SetName("SV Processing Thread");

    std::cout << "SV Processing Thread started" << std::endl;

    int num_packets = 96;

    auto filter_exp = wxGetApp().sv_sub.filter_exp;

    while (true)
    {
        // check if the application is shutting down
        {
            wxCriticalSectionLocker locker(wxGetApp().m_critsect);
            if (wxGetApp().m_shuttingDown)
                return NULL;
        }

        // check if just this thread was asked to exit
        if (TestDestroy())
            break;
        
        
        wxGetApp().network_interface.sniff_traffic(num_packets, filter_exp->data(), "process_sv_data", 100);

        for (auto &idx : *wxGetApp().sv_sub.selectedSV_ids)
        {
            auto handler_ptr = wxGetApp().sv_handler.GetSVHandler(idx);
            std::cout << handler_ptr->SV_data_raw.size() << std::endl;
            handler_ptr->ProcessData();
        }

        wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_DATA_SUCCESSFULLY_PROCESSED);
        wxQueueEvent(wxGetApp().GetMainFrame(), event);

        wxThread::Sleep(5000);
    }

    std::cout << "Thread finished!" << std::endl;

    return (wxThread::ExitCode)NULL;
}