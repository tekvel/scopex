#include "main_frame.h"
#include "main.h"

MainFrame::MainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	// this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	m_parent = new wxPanel(this, wxID_ANY);

	// Menu
	m_menubar = new wxMenuBar;
	menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT, wxT("&Quit"));
	m_menubar->Append(menuFile, wxT("&File"));

	menuNetwork = new wxMenu;
	menuNetwork->Append(wxID_NETWORK_DIALOG, wxT("Select &Network"));
	menuNetwork->Append(wxID_SV_DIALOG, wxT("Select &SV"));
	m_menubar->Append(menuNetwork, wxT("&Network Selection"));

	menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&Help"));
	m_menubar->Append(menuHelp, wxT("&About"));
	SetMenuBar(m_menubar);

	m_toolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_toolbar->SetToolBitmapSize(wxSize(16,16));

	wxImage::AddHandler(new wxPNGHandler);

	wxBitmap saveBitmap = wxBitmap(wxT("src/resources/icons/disk.png"), wxBITMAP_TYPE_PNG);
    wxBitmap playBitmap = wxBitmap(wxT("src/resources/icons/play-circle.png"), wxBITMAP_TYPE_PNG);
    wxBitmap stopBitmap = wxBitmap(wxT("src/resources/icons/stop-circle.png"), wxBITMAP_TYPE_PNG);

	toolSave = m_toolbar->AddTool(wxID_SAVE_TOOLBOX, "Save", saveBitmap, "Save");

	m_toolbar->AddSeparator();

	toolPlay = m_toolbar->AddTool(wxID_PLAY_TOOLBOX, "Save", playBitmap, "Play");
	toolStop = m_toolbar->AddTool(wxID_PLAY_TOOLBOX, "Stop", stopBitmap, "Stop");

	m_toolbar->AddSeparator();

	toolComboBox = new wxComboBox( m_toolbar, wxID_COMBO_BOX_TOOLBOX, wxEmptyString, wxDefaultPosition, wxSize(250,15), 0, NULL, wxCB_READONLY);
	toolComboBox->SetSelection( 0 );
	m_toolbar->AddControl(toolComboBox);
	m_toolbar->Realize();

	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);					   // EVT_Quit
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);				   // EVT_About
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNetworkSelect, this, wxID_NETWORK_DIALOG); // EVT_NIFSelection
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSVSelect, this, wxID_SV_DIALOG);		   // EVT_SVSelection

	// Visual content
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	m_up = new upPanel(m_parent);
	topSizer->Add(m_up, 1, wxEXPAND | wxALL, 5);

	m_dp = new downPanel(m_parent);
	topSizer->Add(m_dp, 5, wxEXPAND | wxALL, 5);

	m_parent->SetSizer(topSizer);
	m_parent->Layout();

	m_parent->Centre(wxBOTH);
}

MainFrame::~MainFrame()
{
	// wxLog::SetActiveTarget(m_oldLogger);

	{
		wxCriticalSectionLocker locker(wxGetApp().m_critsect);

		// check if we have any threads running first
		const wxArrayThread &threads = wxGetApp().m_threads;
		size_t count = threads.GetCount();

		if (!count)
			return;

		// set the flag indicating that all threads should exit
		wxGetApp().m_shuttingDown = true;
	}

	// now wait for them to really terminate
	wxGetApp().m_semAllDone.Wait();
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageDialog dialog(this,
						   "Scopex\n"
						   "2024\n",
						   "Tekvel",
						   wxOK | wxICON_INFORMATION);

	dialog.ShowModal();
}

void MainFrame::OnNetworkSelect(wxCommandEvent &event)
{
	NIF_dialog = new NetworkSelectionDialog("Network Selection Dialog");
	// NIF_dialog->Show();
	NIF_dialog->ShowModal();
	NIF_dialog->Destroy();
}

void MainFrame::OnSVSelect(wxCommandEvent &event)
{
	SV_dialog = new SVSelectionDialog(this, "SV Selection Dialog");
	// SV_dialog->Show();
	SV_dialog->ShowModal();
	SV_dialog->Destroy();
}