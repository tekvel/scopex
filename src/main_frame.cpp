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
	menuNetwork->Append(wxID_NETWORK_DIALOG, wxT("&Network Selection"));
	// menuNetwork->Append(THREAD_START_THREAD, wxT("&Thread"));
	m_menubar->Append(menuNetwork, wxT("&Select Network"));

	menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&Help"));
	m_menubar->Append(menuHelp, wxT("About"));
	SetMenuBar(m_menubar);

	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
	// Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnStartThread, this, THREAD_START_THREAD);
	Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnNetworkSelect, this, wxID_NETWORK_DIALOG);

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

void MainFrame::OnStartThread(wxCommandEvent &event)
{
	MyThread *thread = new MyThread;
	if (thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't create thread!");
	}
	wxCriticalSectionLocker enter(wxGetApp().m_critsect);
	wxGetApp().m_threads.Add(thread);
	if (thread->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogError("Can't start thread!");
	}
}

void MainFrame::OnNetworkSelect(wxCommandEvent &event)
{
	NetworkSelectionDialog *dialog = new NetworkSelectionDialog("Network Selection Dialog");
	// dialog->Show();
	dialog->ShowModal();
	dialog->Destroy();
}

NetworkSelectionDialog::NetworkSelectionDialog(const wxString &title)
	: wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(550, 350)),
	  m_choiceBox(nullptr)
{
	wxPanel *panel = new wxPanel(this, -1);
	wxBoxSizer *vbox1 = new wxBoxSizer(wxVERTICAL);

	m_upPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer *vbox2 = new wxBoxSizer(wxVERTICAL);

	m_text1 = new wxStaticText(m_upPanel, wxID_ANY, wxT("Choose Network:"), wxDefaultPosition, wxDefaultSize, 0);
	m_text1->Wrap(-1);
	vbox2->Add(m_text1, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	m_choiceBox = new wxChoice(m_upPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 40));
	vbox2->Add(m_choiceBox, 0, wxALL | wxEXPAND, 5);

	m_upPanel->SetSizer(vbox2);
	m_upPanel->Layout();
	vbox2->Fit(m_upPanel);
	vbox1->Add(m_upPanel, 1, wxEXPAND | wxALL, 5);

	m_downPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxGridSizer *gsizer = new wxGridSizer(0, 2, 0, 0);

	m_buttonCancel = new wxButton(m_downPanel, wxID_DIALOG_CANCEL, wxT("Cancel"), wxPoint(300, 250), wxDefaultSize, 0);
	gsizer->Add(m_buttonCancel, 0, wxALIGN_CENTER | wxALL, 5);
	Bind(wxEVT_BUTTON, &NetworkSelectionDialog::OnCancel, this, wxID_DIALOG_CANCEL);

	m_buttonOK = new wxButton(m_downPanel, wxID_DIALOG_OK, wxT("OK"), wxPoint(300, 450), wxDefaultSize, 0);
	gsizer->Add(m_buttonOK, 0, wxALIGN_CENTER | wxALL, 5);
	Bind(wxEVT_BUTTON, &NetworkSelectionDialog::OnOK, this, wxID_DIALOG_OK);

	m_downPanel->SetSizer(gsizer);
	m_downPanel->Layout();
	gsizer->Fit(m_downPanel);
	vbox1->Add(m_downPanel, 1, wxEXPAND | wxALL, 5);

	panel->SetSizer(vbox1);
	panel->Layout();
	vbox1->Fit(panel);

	InitializeNetworkDevices();

	Centre();
}

// NetworkSelectionDialog::~NetworkSelectionDialog()
// {
// 	Close(true);
// 	this->Destroy();
// }

void NetworkSelectionDialog::InitializeNetworkDevices()
{
	char errbuf[PCAP_ERRBUF_SIZE]; // Buffer to hold error text
	pcap_if_t *interfaces, *temp;
	int i = 0;

	if (pcap_findalldevs(&interfaces, errbuf) == -1)
	{
		std::cerr << "Error in pcap_findalldevs: " << errbuf << std::endl;
		return;
	}

	wxArrayString deviceList;

	deviceList.Add(wxT(" "));

	for (temp = interfaces; temp; temp = temp->next)
	{
		wxString deviceName = wxString::Format(wxT("%d: %s"), ++i, temp->name);
		if (temp->description)
			deviceName += wxString::Format(wxT(" (%s)"), temp->description);
		deviceList.Add(deviceName);
	}

	m_choiceBox->Append(deviceList);
	m_choiceBox->SetSelection(0);

	pcap_freealldevs(interfaces);
}

void NetworkSelectionDialog::OnCancel(wxCommandEvent &event)
{
	// this->Destroy();
	Close(true);
}

void NetworkSelectionDialog::OnOK(wxCommandEvent &event)
{
	int selectionIndex = m_choiceBox->GetSelection();
	wxString selection = m_choiceBox->GetString(selectionIndex);
	wxMessageBox("You selected: " + selection, "Information", wxOK | wxICON_INFORMATION);
	Close(true);
}