#include "main.h"
#include "nif_dialog.h"

NetworkSelectionDialog::NetworkSelectionDialog(const wxString &title)
    : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(550, 400), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox1 = new wxBoxSizer(wxVERTICAL);

    m_upPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *vbox2 = new wxBoxSizer(wxVERTICAL);

    m_panel1 = new wxPanel(m_upPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    m_text1 = new wxStaticText(m_panel1, wxID_ANY, _("Available Network Interfaces:"), wxDefaultPosition, wxDefaultSize, 0);
    m_text1->Wrap(-1);
    hbox->Add(m_text1, 3, wxALIGN_CENTER | wxALL, 5);

    m_buttonUpdate = new wxButton(m_panel1, wxID_NIF_DIALOG_UPDATE, _("Update List"), wxDefaultPosition, wxDefaultSize, 0);
    hbox->Add(m_buttonUpdate, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_panel1->SetSizer(hbox);
    m_panel1->Layout();
    hbox->Fit(m_panel1);
    vbox2->Add(m_panel1, 1, wxEXPAND | wxALL, 5);

    m_listBox1 = new wxListBox(m_upPanel, wxID_ANY, wxDefaultPosition, wxSize(200, 150), 0, NULL, wxLB_SINGLE);
    vbox2->Add(m_listBox1, 3, wxALL | wxEXPAND, 5);

    m_upPanel->SetSizer(vbox2);
    m_upPanel->Layout();
    vbox2->Fit(m_upPanel);
    vbox1->Add(m_upPanel, 3, wxEXPAND | wxALL, 5);

    m_downPpanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxGridSizer *gsizer;
    gsizer = new wxGridSizer(0, 2, 0, 0);

    m_buttonCancel = new wxButton(m_downPpanel, wxID_NIF_DIALOG_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    gsizer->Add(m_buttonCancel, 0, wxALIGN_CENTER | wxALL, 5);

    m_buttonOK = new wxButton(m_downPpanel, wxID_NIF_DIALOG_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0);
    gsizer->Add(m_buttonOK, 0, wxALIGN_CENTER | wxALL, 5);

    m_downPpanel->SetSizer(gsizer);
    m_downPpanel->Layout();
    gsizer->Fit(m_downPpanel);
    vbox1->Add(m_downPpanel, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(vbox1);
    panel->Layout();
    vbox1->Fit(panel);

    Bind(wxEVT_BUTTON, &NetworkSelectionDialog::OnUpdate, this, wxID_NIF_DIALOG_UPDATE); // EVT_UPDATE_SV_DIALOG
    Bind(wxEVT_BUTTON, &NetworkSelectionDialog::OnCancel, this, wxID_NIF_DIALOG_CANCEL); // EVT_CANCEL_SV_DIALOG
    Bind(wxEVT_BUTTON, &NetworkSelectionDialog::OnOK, this, wxID_NIF_DIALOG_OK);         // EVT_OK_SV_DIALOG

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
    auto device_list = wxGetApp().network_interface.get_device_list();
    wxArrayString deviceList;
    int i = 0;

    if (device_list) // Check if device_list is not nullptr
    {
        for (const auto &device : *device_list) // Dereferencing the shared_ptr to access the vector
        {
            wxString deviceName = wxString::Format(wxT("%d: %s"), ++i, device.c_str());
            deviceList.Add(deviceName);
        }
    }

    m_listBox1->Append(deviceList);
}

void NetworkSelectionDialog::OnCancel(wxCommandEvent &event)
{
    // this->Destroy();
    Close(true);
}

void NetworkSelectionDialog::OnOK(wxCommandEvent &event)
{
    int selectionIndex = m_listBox1->GetSelection();

    bool nif_selected = wxGetApp().network_interface.select_device(selectionIndex);

    if (!nif_selected)
    {
        wxMessageBox(wxT("Network Interface is not selected!\nTry again."), wxT("Error"), wxICON_ERROR);
    }
    else
    {
        // Close the dialog
        Close(true);
    }
}

void NetworkSelectionDialog::OnUpdate(wxCommandEvent &event)
{
    // Clear previous elements
    m_listBox1->Clear();
    InitializeNetworkDevices();
}