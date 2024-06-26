#include "sv_stream_list.h"

SVStreamList::SVStreamList(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxListCtrl(parent, id, pos, size, wxLC_REPORT)
{
    InitList();
}

void SVStreamList::InitList()
{
    AppendColumn("Source", wxLIST_FORMAT_LEFT, 100);
    AppendColumn("Destination", wxLIST_FORMAT_LEFT, 100);
    AppendColumn("Tagged", wxLIST_FORMAT_LEFT, 70);
    AppendColumn("Protocol", wxLIST_FORMAT_LEFT, 100);
}

void SVStreamList::AddStream(const wxString &source, const wxString &destination, bool tagged, const wxString &protocol)
{
    long itemIndex = InsertItem(GetItemCount(), source);
    SetItem(itemIndex, 1, destination);
    SetItem(itemIndex, 2, tagged ? "Yes" : "No");
    SetItem(itemIndex, 3, protocol);
}
