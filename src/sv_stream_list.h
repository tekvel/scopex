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

#ifndef SV_STREAM_LIST_H
#define SV_STREAM_LIST_H

#include <wx/wx.h>
#include <wx/listctrl.h>

class SVStreamList : public wxListCtrl
{
public:
    SVStreamList(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize);
    void AddStream(const wxString &source, 
                   const wxString &destination, 
                   const wxString &APPID,  
                   const wxString &svID,
                   const wxString &noASDU,
                   const wxString &F,
                   const wxString &DatSet);

private:
    void InitList();
};

#endif // SV_STREAM_LIST_H
