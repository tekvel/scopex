#include "main.h"
#include "nif_dialog.h"

NetworkSelectionDialog::NetworkSelectionDialog(const wxString &title)
    : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(550, 350))
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

    auto device_list = network_interface.get_device_list(); // This returns a shared_ptr to a vector of strings
    wxArrayString deviceList;
    int i = 0;

    deviceList.Add(wxT(" ")); // Adding an initial blank option

    if (device_list) // Check if device_list is not nullptr
    {
        for (const auto &device : *device_list) // Dereferencing the shared_ptr to access the vector
        {
            wxString deviceName = wxString::Format(wxT("%d: %s"), ++i, device.c_str());
            deviceList.Add(deviceName);
        }
    }

    m_choiceBox->Append(deviceList);
    m_choiceBox->SetSelection(0);
}

void NetworkSelectionDialog::OnCancel(wxCommandEvent &event)
{
    // this->Destroy();
    Close(true);
}

void NetworkSelectionDialog::OnOK(wxCommandEvent &event)
{
    int selectionIndex = m_choiceBox->GetSelection();

    network_interface.select_device(selectionIndex - 1);

    /*packet = pcap_next(handle, &header);
    std::cout << "Lenght of packet: " << header.len << std::endl;
    std::cout << "Time stamp, sec: " << header.ts.tv_sec << std::endl;
    std::cout << "Time stamp, nsec: " << header.ts.tv_usec << std::endl;

    auto *eth = reinterpret_cast<const ethernet_header *>(packet);

    // Check is frame tagged or not
    if (ntohs(eth->ether_type) == 0x8100)
    {
        std::cout << "Frame is tagged" << std::endl;
        auto *tag_eth = reinterpret_cast<const tag_ethernet_header *>(packet);
        std::cout << "VLAN Identifier: 0x" << (ntohs(tag_eth->tci) & 0x0FFF) << std::endl;
        std::cout << "Priority code point: 0x" << ((ntohs(tag_eth->tci) >> 13) & 0x0007) << std::endl;
        std::cout << "EtherType: 0x" << std::hex << ntohs(tag_eth->ether_type) << std::endl;
    }
    else
    {
        std::cout << "Frame is not tagged" << std::endl;
        std::cout << "EtherType: 0x" << std::hex << ntohs(eth->ether_type) << std::endl;
    }
    pcap_close(handle);*/

    // Close the dialog
    Close(true);
}