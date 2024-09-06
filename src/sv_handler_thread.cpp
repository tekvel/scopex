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
    SetName("SV Capture Thread");

    std::cout << "SV Capture Thread started" << std::endl;

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }

    auto filter_exp = wxGetApp().sv_sub.filter_exp;
    int error = wxGetApp().network_interface.start_capture(filter_exp->data(), "process_sv_data");
    std::cerr << "Error in start_capture: " << error << std::endl;

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);
        if (wxGetApp().m_shuttingDown)
            return NULL;
    }
    
    std::cout << "Thread finished!" << std::endl;

    return (wxThread::ExitCode)NULL;
}

void SVHandlerThread::Stop()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);
    if (wxGetApp().network_interface.isCapturing)
    {
        wxGetApp().network_interface.stop_capture();
        wxGetApp().GetMainFrame()->capture_thread = nullptr;
    }
}

//--------------------SV Process Thread--------------------//

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