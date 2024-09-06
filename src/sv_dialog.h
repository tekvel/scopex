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

#ifndef SV_DIALOG_H
#define SV_DIALOG_H

#include <wx/wx.h>
#include <iostream>

#include "sv_subs_factory.h"
#include "sv_stream_list.h"

class SVSelectionDialog : public wxDialog
{
public:
    SVSelectionDialog(wxWindow *parent, const wxString &title);
    ~SVSelectionDialog();

private:
    void OnOK(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
    void OnUpdate(wxCommandEvent &event);
    void OnSearchComplete(wxThreadEvent &event);

    void UpdateSVList();

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
    wxID_EVT_SEARCH_COMPLETED,
};

#endif