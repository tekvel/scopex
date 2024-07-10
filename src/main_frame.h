#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wxprec.h>
#include <wx/menu.h>
#include <wx/bitmap.h>
#include <wx/image.h>

#include "panels.h"
#include "sv_handler.h"
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
    wxToolBar *m_toolbar;
    wxToolBarToolBase* toolSave;
    wxToolBarToolBase* toolPlay;
    wxToolBarToolBase* toolStop;
    wxComboBox* toolComboBox;

    NetworkSelectionDialog *NIF_dialog;
    SVSelectionDialog *SV_dialog;

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnStartThread(wxCommandEvent &event);
    void OnNetworkSelect(wxCommandEvent &event);
    void OnSVSelect(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnPlay(wxCommandEvent &event);
    void OnStop(wxCommandEvent &event);
    void OnComboBoxSelect(wxCommandEvent &event);

    void RefreshPanels();

    int num_of_drawingPanels;
private:
};

enum
{
    wxID_NETWORK_DIALOG = 201,
    wxID_SV_DIALOG,
    wxID_SAVE_TOOLBOX,
    wxID_PLAY_TOOLBOX,
    wxID_STOP_TOOLBOX,
    wxID_COMBO_BOX_TOOLBOX
};

#endif