#include "panels.h"
#include <wx/wx.h>
#include <wx/stattext.h>
#include "main_frame.h"

upPanel::upPanel(wxPanel* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL )
{
    m_parent = parent;
    m_gsizer1 = new wxGridSizer(0,2,0,0);

    m_textX = new wxStaticText( this, wxID_ANY, _("Change X-scaling:"), wxDefaultPosition, wxDefaultSize, 0);
    m_textX->Wrap( -1 );
    m_gsizer1->Add( m_textX, 1, wxALIGN_CENTER|wxALL, 5 );

    m_textY = new wxStaticText( this, wxID_ANY, _("Change Y-scaling:"), wxDefaultPosition, wxDefaultSize, 0);
    m_textY->Wrap( -1 );
    m_gsizer1->Add( m_textY, 1, wxALIGN_CENTER|wxALL, 5 );

    m_sliderX = new wxSlider( this, ID_X, 10, 2, 202, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_gsizer1->Add( m_sliderX, 1, wxALL|wxEXPAND, 5 );

	m_sliderY = new wxSlider( this, ID_Y, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_gsizer1->Add( m_sliderY, 1, wxALL|wxEXPAND, 5 );

    this->SetSizer(m_gsizer1);
    this->Layout();
    m_gsizer1->Fit(this);

    Bind(wxEVT_SLIDER, &upPanel::OnXScaleChanged, this, ID_X);
    Bind(wxEVT_SLIDER, &upPanel::OnYScaleChanged, this, ID_Y);

}

void upPanel::OnXScaleChanged(wxCommandEvent& event){
    float xScale = event.GetInt() / 100.0;
    MainFrame* frame = (MainFrame *) m_parent->GetParent();
    frame->m_dp->m_sinPanel->SetScale(xScale, frame->m_dp->m_sinPanel->GetYScale());
}

void upPanel::OnYScaleChanged(wxCommandEvent& event){
    float yScale = event.GetInt() / 100.0;
    MainFrame* frame = (MainFrame *) m_parent->GetParent();
    frame->m_dp->m_sinPanel->SetScale(frame->m_dp->m_sinPanel->GetXScale(), yScale);
}

downPanel::downPanel(wxPanel* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL )
{
    m_bsizer2 = new wxBoxSizer(wxHORIZONTAL);

    m_vecPanel = new VectorPanel( this );
    m_bsizer2->Add( m_vecPanel, 1, wxALIGN_CENTER|wxALL|wxSHAPED, 15 );

    m_sinPanel = new SinusoidalPanel( this );
    m_bsizer2->Add( m_sinPanel, 3, wxALL|wxEXPAND, 15);
    
    this->SetSizer(m_bsizer2);
    this->Layout();
    m_bsizer2->Fit(this);
}

