#ifndef NIF_DIALOG_H
#define NIF_DIALOG_H

#include <wx/wx.h>

#include <iostream>
#include <cstdint>
#include <arpa/inet.h>

#include "network_interface.h"

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

    nif network_interface;
};

enum
{
    wxID_DIALOG_OK = 301,
    wxID_DIALOG_CANCEL = 302,

};

#endif