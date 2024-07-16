#ifndef MAIN_H
#define MAIN_H
#include <wx/wx.h>
#include <wx/thread.h>

#include "main_frame.h"
#include "network_interface.h"
#include "sv_subs_factory.h"
#include "sv_handler_factory.h"

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

    NIF network_interface;

    SVSubscribe sv_sub;

    SVHandlerFactory sv_handler;

    std::chrono::time_point<std::chrono::steady_clock> start;

    MainFrame *GetMainFrame() { return frame; }

private:
    MainFrame *frame;
};

wxDECLARE_APP(MyApp);

#endif