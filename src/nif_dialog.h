/*
 * ScopeX
 * Copyright (C) 2024 Tekvel R&D Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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