#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include "panels.h"
#include <wx/wxprec.h>

class Communicate : public wxFrame
{

public:

    Communicate( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 970,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

    upPanel* m_up;
    downPanel* m_dp;
    wxPanel *m_parent;

};

#endif