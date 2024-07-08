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
