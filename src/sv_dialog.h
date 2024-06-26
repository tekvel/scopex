#ifndef SV_DIALOG_H
#define SV_DIALOG_H

#include <wx/wx.h>
#include <iostream>

#include "sv_subs_factory.h"
#include "sv_stream_list.h"

class SVSelectionDialog : public wxDialog
{
public:
    SVSelectionDialog(const wxString &title);
    ~SVSelectionDialog();

private:
    void OnOK(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
    void OnUpdate(wxCommandEvent &event);

    wxPanel *m_upPanel;
    wxPanel *m_panel1;
    wxStaticText *m_text1;
    wxButton *m_buttonUpdate;
    SVStreamList *m_svStreamList;
    wxPanel *m_downPpanel;
    wxButton *m_buttonCancel;
    wxButton *m_buttonOK;
};

enum
{
    wxID_SV_DIALOG_OK = 401,
    wxID_SV_DIALOG_CANCEL,
    wxID_SV_DIALOG_UPDATE,

};

#endif