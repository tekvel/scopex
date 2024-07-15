#include "panels.h"
#include <wx/wx.h>
#include <wx/stattext.h>
#include "main_frame.h"

upPanel::upPanel(wxPanel *parent, int num_of_drawingPanels) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
                                                              num_of_sinPanels(num_of_drawingPanels)
{
    m_parent = parent;
    m_gsizer1 = new wxGridSizer(0, 2, 0, 0);

    m_textX = new wxStaticText(this, wxID_ANY, _("Change X-scaling:"), wxDefaultPosition, wxDefaultSize, 0);
    m_textX->Wrap(-1);
    m_gsizer1->Add(m_textX, 1, wxALIGN_CENTER | wxALL, 5);

    m_textY = new wxStaticText(this, wxID_ANY, _("Change Y-scaling:"), wxDefaultPosition, wxDefaultSize, 0);
    m_textY->Wrap(-1);
    m_gsizer1->Add(m_textY, 1, wxALIGN_CENTER | wxALL, 5);

    m_sliderX = new wxSlider(this, ID_X, 10, 2, 202, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    m_gsizer1->Add(m_sliderX, 1, wxALL | wxEXPAND, 5);

    m_sliderY = new wxSlider(this, ID_Y, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    m_gsizer1->Add(m_sliderY, 1, wxALL | wxEXPAND, 5);

    this->SetSizer(m_gsizer1);
    this->Layout();
    m_gsizer1->Fit(this);

    Bind(wxEVT_SLIDER, &upPanel::OnXScaleChanged, this, ID_X);
    Bind(wxEVT_SLIDER, &upPanel::OnYScaleChanged, this, ID_Y);
}

void upPanel::OnXScaleChanged(wxCommandEvent &event)
{
    float xScale = event.GetInt() / 100.0;
    MainFrame *frame = (MainFrame *)m_parent->GetParent();
    downPanel *dPanel = (downPanel *)frame->m_dp;
    if (num_of_sinPanels == 1)
    {
        dPanel->m_sinPanel1->SetScale(xScale, dPanel->m_sinPanel1->GetYScale());
    }
    else if (num_of_sinPanels == 2)
    {
        dPanel->m_sinPanel1->SetScale(xScale, dPanel->m_sinPanel1->GetYScale());
        dPanel->m_sinPanel2->SetScale(xScale, dPanel->m_sinPanel2->GetYScale());
    }
}

void upPanel::OnYScaleChanged(wxCommandEvent &event)
{
    float yScale = event.GetInt() / 100.0;
    MainFrame *frame = (MainFrame *)m_parent->GetParent();
    downPanel *dPanel = (downPanel *)frame->m_dp;
    if (num_of_sinPanels == 1)
    {
        dPanel->m_sinPanel1->SetScale(dPanel->m_sinPanel1->GetXScale(), yScale);
    }
    else if (num_of_sinPanels == 2)
    {
        dPanel->m_sinPanel1->SetScale(dPanel->m_sinPanel1->GetXScale(), yScale);
        dPanel->m_sinPanel2->SetScale(dPanel->m_sinPanel2->GetXScale(), yScale);
    }
    
}

downPanel::downPanel(wxPanel *parent, int num_of_drawingPanels) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL), 
                                                                  num_of_sinPanels(num_of_drawingPanels)
{
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    m_leftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL);

    m_vecPanel = new VectorPanel(m_leftPanel);
    vbox1->Add( m_vecPanel, 1, wxALIGN_CENTER|wxALL|wxSHAPED, 15 );

    m_leftPanel->SetSizer(vbox1);
	m_leftPanel->Layout();
    vbox1->Fit(m_leftPanel);
    hbox->Add( m_leftPanel, 1, wxEXPAND | wxALL, 5 );

    m_rightPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* vbox2 = new wxBoxSizer( wxVERTICAL );

    if (num_of_sinPanels == 1)
    {
        // m_sinPanel1 = new SinusoidalPanel(m_rightPanel);
        // vbox2->Add( m_sinPanel1, 1, wxALL|wxEXPAND, 15 );
        m_drawingPanel = new DrawingPanel(m_rightPanel);
        vbox2->Add( m_drawingPanel, 1, wxALL|wxEXPAND, 15 );
    }
    else if (num_of_sinPanels == 2)
    {
        m_sinPanel1 = new SinusoidalPanel(m_rightPanel);
        vbox2->Add( m_sinPanel1, 1, wxALL|wxEXPAND, 15 );

        m_sinPanel2 = new SinusoidalPanel(m_rightPanel);
        vbox2->Add( m_sinPanel2, 1, wxALL|wxEXPAND, 15 );
    }

    m_rightPanel->SetSizer( vbox2 );
	m_rightPanel->Layout();
	vbox2->Fit( m_rightPanel );

    hbox->Add( m_rightPanel, 3, wxEXPAND | wxALL, 5 );

    this->SetSizer(hbox);
    this->Layout();
    hbox->Fit(this);
}
