#include "sv_handler.h"
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

    int num_packets = 2;

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
        
        
        wxGetApp().network_interface.sniff_traffic(num_packets, filter_exp->data(), "got_packet", 100);

        wxThread::Sleep(2000);
    }

    std::cout << "Thread finished!" << std::endl;

    return NULL;
}