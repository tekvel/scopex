#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wxprec.h>
#include <wx/menu.h>
#include <pcap.h>
#include "panels.h"
#include "threads.h"

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

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnStartThread(wxCommandEvent &event);
    void OnNetworkSelect(wxCommandEvent &event);
};

class NetworkSelectionDialog : public wxDialog
{
public:
    NetworkSelectionDialog(const wxString &title);
    // virtual ~NetworkSelectionDialog();

private:
    void InitializeNetworkDevices();
    void OnOK(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);

    wxPanel *m_upPanel;
    wxPanel *m_downPanel;
    wxStaticText *m_text1;
    wxChoice *m_choiceBox;
    wxButton *m_buttonOK;
    wxButton *m_buttonCancel;
};

enum
{
    THREAD_START_THREAD = 201,
    wxID_NETWORK_DIALOG,
    wxID_CHOICE_BOX,
    wxID_DIALOG_OK,
    wxID_DIALOG_CANCEL,

};

#endif