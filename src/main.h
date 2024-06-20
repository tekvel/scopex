#ifndef MAIN_H
#define MAIN_H
#include <wx/wx.h>
#include "wx/thread.h"

WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

class MyApp : public wxApp
{
public:
    MyApp();
    virtual ~MyApp() {}

    virtual bool OnInit() wxOVERRIDE;

    // critical section protects access to all of the fields below
    wxCriticalSection m_critsect;

    // all the threads currently alive - as soon as the thread terminates, it's
    // removed from the array
    wxArrayThread m_threads;

    // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;

    // indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;
};

wxDECLARE_APP(MyApp);

#endif