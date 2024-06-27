#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wxprec.h>
#include <wx/menu.h>

#include "panels.h"
#include "threads.h"
// Dialog windows
#include "nif_dialog.h"
#include "sv_dialog.h"

class MainFrame : public wxFrame
{

public:
    MainFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(970, 600), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    virtual ~MainFrame();

    upPanel *m_up;
    downPanel *m_dp;
    wxPanel *m_parent;
    wxMenuBar *m_menubar;
    wxMenu *menuFile;
    wxMenu *menuHelp;
    wxMenu *menuNetwork;

    NetworkSelectionDialog *NIF_dialog;
    SVSelectionDialog *SV_dialog;

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnStartThread(wxCommandEvent &event);
    void OnNetworkSelect(wxCommandEvent &event);
    void OnSVSelect(wxCommandEvent &event);
};

enum
{
    THREAD_START_THREAD = 201,
    wxID_NETWORK_DIALOG,
    wxID_SV_DIALOG,
};

#endif