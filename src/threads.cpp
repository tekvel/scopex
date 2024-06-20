#include "threads.h"
#include "main.h"
#include <iostream>

MyThread::MyThread() : wxThread()
{
    m_count = 0;
}

MyThread::~MyThread()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);

    wxArrayThread &threads = wxGetApp().m_threads;
    threads.Remove(this);

    if (threads.IsEmpty())
    {
        // signal the main thread that there are no more threads left if it is
        // waiting for us
        if (wxGetApp().m_shuttingDown)
        {
            wxGetApp().m_shuttingDown = false;

            wxGetApp().m_semAllDone.Post();
        }
    }
}

wxThread::ExitCode MyThread::Entry()
{
    SetName("My Thread");

    std::cout << "Thread started with priority: " << GetPriority() << std::endl;

    for (m_count = 0; m_count < 10; m_count++)
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

        std::cout << "Thread progress: " << m_count << std::endl;

        wxThread::Sleep(500);
    }

    std::cout << "Thread finished!" << std::endl;

    return NULL;
}
