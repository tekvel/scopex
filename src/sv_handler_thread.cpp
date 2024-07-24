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

    int num_packets = 192;

    auto filter_exp = wxGetApp().sv_sub.filter_exp;

    int timeout_expired = 0;

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
        
        if (wxGetApp().network_interface.sniff_traffic(num_packets, filter_exp->data(), "process_sv_data", 200) == -2)
        {
            timeout_expired += 1;
        }
        else
        {
            timeout_expired = 0;
        }
        if (timeout_expired == 20 || wxGetApp().network_interface.noIrrelevantFrames > num_packets * 20)
        {
            wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_DATA_NOT_FOUND);
            wxQueueEvent(wxGetApp().GetMainFrame(), event);
            break;
        }

        // wxThread::Sleep(10);
    }

    std::cout << "Thread finished!" << std::endl;

    return (wxThread::ExitCode)NULL;
}

SVProcessThread::SVProcessThread()
{
}

SVProcessThread::~SVProcessThread()
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

wxThread::ExitCode SVProcessThread::Entry()
{
    SetName("SV Processing 2 Thread");

    std::cout << "SV Processing 2 Thread started" << std::endl;
   
    // check if the application is shutting down
    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }

    auto idx = wxGetApp().sv_sub.selectedSV_id_main;
    auto handler_ptr = wxGetApp().sv_handler.GetSVHandler(*idx);
    handler_ptr->ProcessData();
    handler_ptr->SAnalyzer.CalculateParameters();
    // handler_ptr->SAnalyzer.calculateAmplitudeAndPhase();
        
    // wxThread::Sleep(50);

    wxThreadEvent *event = new wxThreadEvent(wxEVT_THREAD, wxID_EVT_DATA_SUCCESSFULLY_PROCESSED);
    wxQueueEvent(wxGetApp().GetMainFrame(), event);

    std::cout << "Process 2 Thread finished!" << std::endl;

    return (wxThread::ExitCode)NULL;
}