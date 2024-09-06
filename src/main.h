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