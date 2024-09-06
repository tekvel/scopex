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

#include "sv_stream_list.h"

SVStreamList::SVStreamList(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxListCtrl(parent, id, pos, size, wxLC_REPORT)
{
    InitList();
}

void SVStreamList::InitList()
{
    AppendColumn("Source", wxLIST_FORMAT_CENTER, 150);
    AppendColumn("Destination", wxLIST_FORMAT_CENTER, 150);
    AppendColumn("APPID", wxLIST_FORMAT_CENTER, 80);
    AppendColumn("svID", wxLIST_FORMAT_CENTER, 180);
    AppendColumn("noASDU", wxLIST_FORMAT_CENTER, 80);
    AppendColumn("F, Hz", wxLIST_FORMAT_CENTER, 80);
    AppendColumn("DatSet", wxLIST_FORMAT_CENTER, 80);
}

void SVStreamList::AddStream(const wxString &source, 
                             const wxString &destination, 
                             const wxString &APPID,  
                             const wxString &svID,
                             const wxString &noASDU,
                             const wxString &F,
                             const wxString &DatSet)
{
    long itemIndex = InsertItem(GetItemCount(), source);
    SetItem(itemIndex, 1, destination);
    SetItem(itemIndex, 2, APPID);
    SetItem(itemIndex, 3, svID);
    SetItem(itemIndex, 4, noASDU);
    SetItem(itemIndex, 5, F);
    SetItem(itemIndex, 6, DatSet);
}
