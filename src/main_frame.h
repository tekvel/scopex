#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wxprec.h>
#include <wx/menu.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/notebook.h>

#include "osc_panel.h"
#include "sv_handler_thread.h"
// Dialog windows
#include "nif_dialog.h"
#include "sv_dialog.h"

class MainFrame : public wxFrame
{

public:
    MainFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(970, 600), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
    virtual ~MainFrame();

    wxNotebook *m_notebook1;
    OscilloscopePanel *m_oscp;
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

    wxTimer timer1;

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnStartThread(wxCommandEvent &event);
    void OnNetworkSelect(wxCommandEvent &event);
    void OnSVSelect(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void OnPlay(wxCommandEvent &event);
    void OnStop(wxCommandEvent &event);
    void OnComboBoxSelect(wxCommandEvent &event);

    void OnDataProcessed(wxThreadEvent &event);
    void OnDataNotFound(wxThreadEvent &event);

    void OnTimer(wxTimerEvent &event);

    void RefreshPanels();
    void EnableTools(bool enable);

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
    wxID_COMBO_BOX_TOOLBOX,
    wxID_EVT_DATA_SUCCESSFULLY_PROCESSED,
    wxID_EVT_DATA_NOT_FOUND,
    wxID_EVT_TIMER_DONE
};

#endif