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
    AppendColumn("Length", wxLIST_FORMAT_CENTER, 80);
    AppendColumn("svID", wxLIST_FORMAT_CENTER, 150);
    AppendColumn("noASDU", wxLIST_FORMAT_CENTER, 80);
    AppendColumn("DatSet", wxLIST_FORMAT_CENTER, 80);
}

void SVStreamList::AddStream(const wxString &source, 
                             const wxString &destination, 
                             const wxString &APPID, 
                             const wxString &Length, 
                             const wxString &svID,
                             const wxString &noASDU,
                             const wxString &DatSet)
{
    long itemIndex = InsertItem(GetItemCount(), source);
    SetItem(itemIndex, 1, destination);
    SetItem(itemIndex, 2, APPID);
    SetItem(itemIndex, 3, Length);
    SetItem(itemIndex, 4, svID);
    SetItem(itemIndex, 5, noASDU);
    SetItem(itemIndex, 6, DatSet);
}
