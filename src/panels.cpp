#include "panels.h"
#include <wx/wx.h>
#include <wx/stattext.h>
#include "communicate.h"

upPanel::upPanel(wxPanel* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL )
{
    m_parent = parent;
    m_gsizer1 = new wxGridSizer(0,2,0,0);

    m_sliderX = new wxSlider( this, ID_X, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_gsizer1->Add( m_sliderX, 1, wxALL|wxEXPAND, 15 );

	m_sliderY = new wxSlider( this, ID_Y, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_gsizer1->Add( m_sliderY, 1, wxALL|wxEXPAND, 15 );

	m_textCtrl1 = new wxTextCtrl( this, ID_control1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_gsizer1->Add( m_textCtrl1, 1, wxALIGN_CENTER|wxALL, 5 );

	m_textCtrl2 = new wxTextCtrl( this, ID_control2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_gsizer1->Add( m_textCtrl2, 1, wxALIGN_CENTER|wxALL, 5 );

    this->SetSizer(m_gsizer1);
    this->Layout();
    m_gsizer1->Fit(this);

    Bind(wxEVT_SLIDER, &upPanel::OnXScaleChanged, this, ID_X);
    Bind(wxEVT_SLIDER, &upPanel::OnYScaleChanged, this, ID_Y);

    Bind(wxEVT_TEXT, &upPanel::OnPivotXChanged, this, ID_control1);
    Bind(wxEVT_TEXT, &upPanel::OnPivotYChanged, this, ID_control2);

}

void upPanel::OnXScaleChanged(wxCommandEvent& event){
    float xScale = event.GetInt() / 100.0;
    Communicate* comm = (Communicate *) m_parent->GetParent();
    comm->m_dp->m_sinPanel->SetScale(xScale, comm->m_dp->m_sinPanel->GetYScale());
}

void upPanel::OnYScaleChanged(wxCommandEvent& event){
    float yScale = event.GetInt() / 100.0;
    Communicate* comm = (Communicate *) m_parent->GetParent();
    comm->m_dp->m_sinPanel->SetScale(comm->m_dp->m_sinPanel->GetXScale(), yScale);
}

void upPanel::OnPivotXChanged(wxCommandEvent& event){
    long pivotX;
    Communicate* comm = (Communicate *) m_parent->GetParent();
    if (m_textCtrl1->GetValue().ToLong(&pivotX)) {
        wxPoint pivot = comm->m_dp->m_sinPanel->GetPivotPoint();
        pivot.x = pivotX;
        comm->m_dp->m_sinPanel->SetPivotPoint(pivot);
    }
}

void upPanel::OnPivotYChanged(wxCommandEvent& event){
    long pivotY;
    Communicate* comm = (Communicate *) m_parent->GetParent();
    if (m_textCtrl1->GetValue().ToLong(&pivotY)) {
        wxPoint pivot = comm->m_dp->m_sinPanel->GetPivotPoint();
        pivot.y = pivotY;
        comm->m_dp->m_sinPanel->SetPivotPoint(pivot);
    }
}


downPanel::downPanel(wxPanel* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL )
{
    m_bsizer2 = new wxBoxSizer(wxHORIZONTAL);

    m_bitmap1 = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
    m_bsizer2->Add( m_bitmap1, 1, wxALIGN_CENTER|wxALL|wxSHAPED, 15 );

    m_sinPanel = new SinusoidalPanel(this);
    m_bsizer2->Add( m_sinPanel, 3, wxALL|wxEXPAND, 15);
    
    this->SetSizer(m_bsizer2);
    this->Layout();
    m_bsizer2->Fit(this);
}

