#include "communicate.h"

Communicate::Communicate( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	// this->SetSizeHints( wxDefaultSize, wxDefaultSize );
  	m_parent = new wxPanel(this, wxID_ANY);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	m_up = new upPanel(m_parent);
	topSizer->Add( m_up, 1, wxEXPAND | wxALL, 5 );

	m_dp = new downPanel(m_parent);
	topSizer->Add( m_dp, 5, wxEXPAND | wxALL, 5 );


	m_parent->SetSizer( topSizer );
	m_parent->Layout();

	m_parent->Centre( wxBOTH );
}