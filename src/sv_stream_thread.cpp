#include "sv_stream_thread.h"
#include "main.h"

SVSearchThread::SVSearchThread() : wxThread()
{
}

SVSearchThread::~SVSearchThread()
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

wxThread::ExitCode SVSearchThread::Entry()
{
    SetName("SV Searching Thread");

    // check if the application is shutting down
    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }

    // // check if just this thread was asked to exit
    // if (TestDestroy())
    //     break;

    // char filter_exp[] = "ether proto 0x0800";
    // char filter_exp[] = "arp";
    // char filter_exp[] = "ether proto 0x88ba";
    char filter_exp[] = "";

    wxGetApp().network_interface.sniff_traffic(10, filter_exp, 100);

    wxThread::Sleep(5000);
    std::cout << "\nHello from thread" << std::endl;

    return NULL;
}