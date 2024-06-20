#include "main.h"
#include "main_frame.h"

wxIMPLEMENT_APP(MyApp);

MyApp::MyApp()
{
    m_shuttingDown = false;
}

bool MyApp::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MainFrame *frame = new MainFrame(nullptr, wxID_ANY, wxT("Scopex"));
    frame->Show(true);

    return true;
}
