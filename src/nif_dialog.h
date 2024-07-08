#ifndef NIF_DIALOG_H
#define NIF_DIALOG_H

#include <wx/wx.h>

#include <iostream>
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
    void OnUpdate(wxCommandEvent &event);

    wxPanel *m_upPanel;
    wxPanel *m_panel1;
    wxStaticText *m_text1;
    wxButton *m_buttonUpdate;
    wxListBox *m_listBox1;
    wxPanel *m_downPpanel;
    wxButton *m_buttonOK;
    wxButton *m_buttonCancel;
};

enum
{
    wxID_NIF_DIALOG_OK = 301,
    wxID_NIF_DIALOG_CANCEL,
    wxID_NIF_DIALOG_UPDATE,

};

#endif