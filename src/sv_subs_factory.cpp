#include "sv_subs_factory.h"
#include "main.h"

std::shared_ptr<std::vector<std::string>> SVSubscribe::get_sv_list()
{
    SVSearchThread *thread = new SVSearchThread;
    if (thread->Create() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't create thread!" << std::endl;
    }
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
    wxGetApp().m_threads.Add(thread);
    if (thread->Run() != wxTHREAD_NO_ERROR)
    {
        std::cerr << "Can't start thread!" << std::endl;
    }

    return nullptr;
}