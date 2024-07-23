#include "osc_panel.h"
#include <wx/wx.h>
#include <wx/stattext.h>
#include "main_frame.h"

OscilloscopePanel::OscilloscopePanel(wxNotebook *parent, int num_of_drawingPanels) 
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL), 
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
        m_text1 = new wxStaticText(m_rightPanel, wxID_ANY, _("Curents:"), wxDefaultPosition, wxDefaultSize, 0);
        m_text1->Wrap( -1 );
        vbox2->Add( m_text1, 0, wxALL, 5 );

        m_drawingPanel1 = new DrawingPanel(m_rightPanel, 0);
        vbox2->Add( m_drawingPanel1, 1, wxALL|wxEXPAND, 15 );
    }
    else if (num_of_sinPanels == 2)
    {   
        m_text1 = new wxStaticText(m_rightPanel, wxID_ANY, _("Curents:"), wxDefaultPosition, wxDefaultSize, 0);
        m_text1->Wrap( -1 );
        vbox2->Add( m_text1, 0, wxALL, 5 );

        m_drawingPanel1 = new DrawingPanel(m_rightPanel, 0);
        vbox2->Add( m_drawingPanel1, 1, wxALL|wxEXPAND, 15 );

        m_text2 = new wxStaticText(m_rightPanel, wxID_ANY, _("Voltages:"), wxDefaultPosition, wxDefaultSize, 0);
        m_text2->Wrap( -1 );
        vbox2->Add( m_text2, 0, wxALL, 5 );

        m_drawingPanel2 = new DrawingPanel(m_rightPanel, 1);
        vbox2->Add( m_drawingPanel2, 1, wxALL|wxEXPAND, 15 );
    }

    m_rightPanel->SetSizer( vbox2 );
	m_rightPanel->Layout();
	vbox2->Fit( m_rightPanel );

    hbox->Add( m_rightPanel, 3, wxEXPAND | wxALL, 5 );

    this->SetSizer(hbox);
    this->Layout();
    hbox->Fit(this);
}

void OscilloscopePanel::SynchronizeScroll(DrawingPanel *source, int newPosition)
{
    if (num_of_sinPanels == 2)
    {
        if (source == m_drawingPanel1 && m_drawingPanel2 != nullptr)
        {
            m_drawingPanel2->SetScrollPos(wxHORIZONTAL, newPosition, true);
            m_drawingPanel2->Refresh();
            m_drawingPanel2->Update();
        }
        else if (source == m_drawingPanel2 && m_drawingPanel1 != nullptr)
        {
            m_drawingPanel1->SetScrollPos(wxHORIZONTAL, newPosition, true);
            m_drawingPanel1->Refresh();
            m_drawingPanel1->Update();
        }
    }
}

void OscilloscopePanel::SynchronizeXScale(DrawingPanel *source, float newXScale)
{
    if (num_of_sinPanels == 2)
    {
        if (source == m_drawingPanel1 && m_drawingPanel2 != nullptr)
        {
            m_drawingPanel2->xScale = newXScale;
            m_drawingPanel2->Refresh();
            m_drawingPanel2->Update();
        }
        else if (source == m_drawingPanel2 && m_drawingPanel1 != nullptr)
        {
            m_drawingPanel1->xScale = newXScale;
            m_drawingPanel1->Refresh();
            m_drawingPanel1->Update();
        }
    }
}