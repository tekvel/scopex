#include "main_frame.h"
#include "main.h"

MainFrame::MainFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) 
			: wxFrame(parent, id, title, pos, size, style),
			  num_of_drawingPanels(1),
			  timer1(this, wxID_EVT_TIMER_DONE)
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

	// Toolbar
	m_toolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_toolbar->SetToolBitmapSize(wxSize(16,16));

	wxImage::AddHandler(new wxPNGHandler);

	wxBitmap saveBitmap = wxBitmap(wxT("src/resources/icons/disk.png"), wxBITMAP_TYPE_PNG);
    wxBitmap playBitmap = wxBitmap(wxT("src/resources/icons/play-circle.png"), wxBITMAP_TYPE_PNG);
    wxBitmap stopBitmap = wxBitmap(wxT("src/resources/icons/stop-circle.png"), wxBITMAP_TYPE_PNG);

	toolSave = m_toolbar->AddTool(wxID_SAVE_TOOLBOX, "Save", saveBitmap, "Save");

	m_toolbar->AddSeparator();

	toolPlay = m_toolbar->AddTool(wxID_PLAY_TOOLBOX, "Save", playBitmap, "Play");
	toolStop = m_toolbar->AddTool(wxID_STOP_TOOLBOX, "Stop", stopBitmap, "Stop");

	m_toolbar->AddSeparator();

	toolComboBox = new wxComboBox( m_toolbar, wxID_COMBO_BOX_TOOLBOX, wxEmptyString, wxDefaultPosition, wxSize(350,15), 0, NULL, wxCB_READONLY);
	toolComboBox->SetToolTip("Choose SV streams to display");
	m_toolbar->AddControl(toolComboBox);
	m_toolbar->Realize();
	
	EnableTools(false);		// Disable all tools

	// Binding events
	// Menu events
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);					   // EVT_Quit
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);				   // EVT_About
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNetworkSelect, this, wxID_NETWORK_DIALOG); // EVT_NIFSelection
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnSVSelect, this, wxID_SV_DIALOG);		   // EVT_SVSelection

	// Toolbar events
	Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnSave, this, wxID_SAVE_TOOLBOX);						// EVT_Save
	Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnPlay, this, wxID_PLAY_TOOLBOX);						// EVT_Play
	Bind(wxEVT_COMMAND_TOOL_CLICKED, &MainFrame::OnStop, this, wxID_STOP_TOOLBOX);						// EVT_Stop
	Bind(wxEVT_COMBOBOX, &MainFrame::OnComboBoxSelect, this, wxID_COMBO_BOX_TOOLBOX);					// EVT_ComboBoxSelection

	// Thread events
	Bind(wxEVT_THREAD, &MainFrame::OnDataProcessed, this, wxID_EVT_DATA_SUCCESSFULLY_PROCESSED);	// EVT_DATA_PROCESSED
	Bind(wxEVT_THREAD, &MainFrame::OnDataNotFound, this, wxID_EVT_DATA_NOT_FOUND);					// EVT_DATA_NOT_FOUND

	// Timer events
	Bind(wxEVT_TIMER, &MainFrame::OnTimer, this, wxID_EVT_TIMER_DONE);	// EVT_TIMER_DONE

	// Visual content
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	m_up = new upPanel(m_parent, num_of_drawingPanels);
	topSizer->Add(m_up, 1, wxEXPAND | wxALL, 5);

	m_dp = new downPanel(m_parent, num_of_drawingPanels);
	topSizer->Add(m_dp, 5, wxEXPAND | wxALL, 5);

	m_parent->SetSizer(topSizer);
	m_parent->Layout();

	m_parent->Centre(wxBOTH);
}

MainFrame::~MainFrame()
{
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

void MainFrame::RefreshPanels()
{
    // Clear the existing sizer and delete the existing panels
    m_parent->GetSizer()->Clear(true);

    // Create new instances of upPanel and downPanel with the correct number of panels
    m_up = new upPanel(m_parent, num_of_drawingPanels);
    m_parent->GetSizer()->Add(m_up, 1, wxEXPAND | wxALL, 5);

    m_dp = new downPanel(m_parent, num_of_drawingPanels);
    m_parent->GetSizer()->Add(m_dp, 5, wxEXPAND | wxALL, 5);

    // Re-layout the sizer
    m_parent->GetSizer()->Layout();
    m_parent->Layout();
    this->Layout(); // Ensure the main frame also updates its layout
}

void MainFrame::EnableTools(bool enable)
{
	m_toolbar->EnableTool(wxID_PLAY_TOOLBOX, enable);
	m_toolbar->EnableTool(wxID_STOP_TOOLBOX, enable);
	m_toolbar->EnableTool(wxID_SAVE_TOOLBOX, enable);
	m_toolbar->EnableTool(wxID_COMBO_BOX_TOOLBOX, enable);
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
	NIF_dialog->ShowModal();
	NIF_dialog->Destroy();
}

void MainFrame::OnSVSelect(wxCommandEvent &event)
{
	SV_dialog = new SVSelectionDialog(this, "SV Selection Dialog");
	SV_dialog->ShowModal();
	SV_dialog->Destroy();
}

void MainFrame::OnSave(wxCommandEvent &event)
{

}

void MainFrame::OnPlay(wxCommandEvent &event)
{
	SVHandlerThread *thread = new SVHandlerThread;
	if (thread->Create() != wxTHREAD_NO_ERROR)
	{
		std::cerr << "Can't create SVHandler thread!" << std::endl;
		return;
	}
	wxCriticalSectionLocker enter(wxGetApp().m_critsect);
	wxGetApp().m_threads.Add(thread);
	if (thread->Run() != wxTHREAD_NO_ERROR)
	{
		std::cerr << "Can't start SVHandler thread!" << std::endl;
		return;
	}
	m_toolbar->EnableTool(wxID_PLAY_TOOLBOX, false);		// Disable Play tool
	m_toolbar->EnableTool(wxID_COMBO_BOX_TOOLBOX, false);	// Disable ComboBox tool
}

void MainFrame::OnStop(wxCommandEvent &event)
{
	wxArrayThread &threads = wxGetApp().m_threads;
	if (!threads.IsEmpty())
	{
		wxGetApp().m_shuttingDown = true;
		m_toolbar->EnableTool(wxID_PLAY_TOOLBOX, true);			// Enable Play tool
		m_toolbar->EnableTool(wxID_COMBO_BOX_TOOLBOX, true);	// Enable ComboBox tool
	}
}

void MainFrame::OnComboBoxSelect(wxCommandEvent &event)
{
	int idx = toolComboBox->GetSelection();
	auto id = wxGetApp().sv_sub.selectedSV_ids->at(idx);
	auto it = wxGetApp().sv_sub.sv_list->begin();
	std::advance(it, id);

	if (it != wxGetApp().sv_sub.sv_list->end())
	{
		auto DatSet = it->DatSet;

		if ((DatSet == 8) || (DatSet == 6))
		{
			if (num_of_drawingPanels == 1)
			{
				num_of_drawingPanels = 2;
				RefreshPanels();
			}
		}
		else
		{
			if (num_of_drawingPanels == 2)
			{
				num_of_drawingPanels = 1;
				RefreshPanels();
			}
		}
	}
}

void MainFrame::OnDataProcessed(wxThreadEvent &event)
{
	if (num_of_drawingPanels == 1)
	{
		m_dp->m_drawingPanel->isGreen = true;
		m_dp->m_drawingPanel->Refresh();
		m_dp->m_drawingPanel->Update();
	}
	else if (num_of_drawingPanels == 2)
	{

	}
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - wxGetApp().start).count();
	std::cout << "Elapsed time: " << elapsed << std::endl;

	timer1.StartOnce(250);
}

void MainFrame::OnDataNotFound(wxThreadEvent &event)
{
	wxArrayThread &threads = wxGetApp().m_threads;
	if (!threads.IsEmpty())
	{
		wxGetApp().m_shuttingDown = true;
	}

	m_toolbar->EnableTool(wxID_PLAY_TOOLBOX, true);			// Enable Play tool
	m_toolbar->EnableTool(wxID_COMBO_BOX_TOOLBOX, true);	// Enable ComboBox tool

	// Reinitialize SV_Handler attributes
	for (auto &idx : *wxGetApp().sv_sub.selectedSV_ids)
	{
		auto handler_ptr = wxGetApp().sv_handler.GetSVHandler(idx);
		handler_ptr->InitializeAttributes();
	}
	// Refresh Drawing Panels
	if (num_of_drawingPanels == 1)
	{
		m_dp->m_drawingPanel->Refresh();
		m_dp->m_drawingPanel->Update();
	}
	else if (num_of_drawingPanels == 2)
	{

	}

	wxMessageBox(wxT("Can't find selected SV stream in network!\n\nChange SV."), wxT("Stop Displaying"), wxICON_STOP);
}

void MainFrame::OnTimer(wxTimerEvent &event)
{
	if (num_of_drawingPanels == 1)
	{
		m_dp->m_drawingPanel->isGreen = false;
		m_dp->m_drawingPanel->Refresh();
		m_dp->m_drawingPanel->Update();
	}
	else if (num_of_drawingPanels == 2)
	{

	}
}