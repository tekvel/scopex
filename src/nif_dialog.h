#ifndef NIF_DIALOG_H
#define NIF_DIALOG_H

#include <wx/wx.h>

#include <iostream>
#include <cstdint>
#include <arpa/inet.h>

class NetworkSelectionDialog : public wxDialog
{
public:
    NetworkSelectionDialog(const wxString &title);
    //~NetworkSelectionDialog();

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
    wxID_NIF_DIALOG_OK = 301,
    wxID_NIF_DIALOG_CANCEL,

};

#endif