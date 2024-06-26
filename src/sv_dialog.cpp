#include "sv_dialog.h"
#include "main.h"

SVSelectionDialog::SVSelectionDialog(const wxString &title)
    : wxDialog(NULL, -1, title, wxDefaultPosition, wxSize(550, 350))
{
    wxPanel *panel = new wxPanel(this, -1);
    wxBoxSizer *vbox1 = new wxBoxSizer(wxVERTICAL);

    m_upPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *vbox2 = new wxBoxSizer(wxVERTICAL);

    m_panel1 = new wxPanel(m_upPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

    m_text1 = new wxStaticText(m_panel1, wxID_ANY, _("Available SV:"), wxDefaultPosition, wxDefaultSize, 0);
    m_text1->Wrap(-1);
    hbox->Add(m_text1, 3, wxALIGN_CENTER | wxALL, 5);

    m_buttonUpdate = new wxButton(m_panel1, wxID_SV_DIALOG_UPDATE, _("Update List"), wxDefaultPosition, wxDefaultSize, 0);
    hbox->Add(m_buttonUpdate, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_panel1->SetSizer(hbox);
    m_panel1->Layout();
    hbox->Fit(m_panel1);
    vbox2->Add(m_panel1, 1, wxEXPAND | wxALL, 5);

    m_svStreamList = new SVStreamList(m_upPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxString source = "00:11:22:33:44:55";
    wxString destination = "66:77:88:99:AA:BB";
    bool tagged = true;
    wxString protocol = "IPv4";
    m_svStreamList->AddStream(source, destination, tagged, protocol);
    m_svStreamList->AddStream(source, destination, tagged, protocol);
    m_svStreamList->AddStream(source, destination, tagged, protocol);
    vbox2->Add(m_svStreamList, 3, wxALL | wxEXPAND, 5);

    m_upPanel->SetSizer(vbox2);
    m_upPanel->Layout();
    vbox2->Fit(m_upPanel);
    vbox1->Add(m_upPanel, 3, wxEXPAND | wxALL, 5);

    m_downPpanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxGridSizer *gsizer;
    gsizer = new wxGridSizer(0, 2, 0, 0);

    m_buttonCancel = new wxButton(m_downPpanel, wxID_SV_DIALOG_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0);
    gsizer->Add(m_buttonCancel, 0, wxALIGN_CENTER | wxALL, 5);

    m_buttonOK = new wxButton(m_downPpanel, wxID_SV_DIALOG_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0);
    gsizer->Add(m_buttonOK, 0, wxALIGN_CENTER | wxALL, 5);

    m_downPpanel->SetSizer(gsizer);
    m_downPpanel->Layout();
    gsizer->Fit(m_downPpanel);
    vbox1->Add(m_downPpanel, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(vbox1);
    panel->Layout();
    vbox1->Fit(panel);

    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnUpdate, this, wxID_SV_DIALOG_UPDATE); // EVT_UPDATE_SV_DIALOG
    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnCancel, this, wxID_SV_DIALOG_CANCEL); // EVT_CANCEL_SV_DIALOG
    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnOK, this, wxID_SV_DIALOG_OK);         // EVT_OK_SV_DIALOG

    Centre();
}

SVSelectionDialog::~SVSelectionDialog()
{
}

void SVSelectionDialog::OnCancel(wxCommandEvent &event)
{
    // Close the dialog
    Close(true);
}

void SVSelectionDialog::OnOK(wxCommandEvent &event)
{
    wxArrayInt selectedItems;
    long item = -1;
    while ((item = m_svStreamList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
        selectedItems.Add(item);
    }

    int n = selectedItems.GetCount();
    if (n == 0)
    {
        wxMessageBox(wxT("SV Stream is not selected!"), wxT("Error!"), wxICON_ERROR);
    }
    else
    {
        std::cout << "Number of selected items: " << n << std::endl;

        // Optionally, you can retrieve more details about each selected item
        for (int i = 0; i < n; ++i)
        {
            long index = selectedItems[i];
            wxString source = m_svStreamList->GetItemText(index);
            wxString destination = m_svStreamList->GetItemText(index, 1);
            wxString tagged = m_svStreamList->GetItemText(index, 2);
            wxString protocol = m_svStreamList->GetItemText(index, 3);

            std::cout << "Selected Item " << i << ": " << source << ", " << destination << ", " << tagged << ", " << protocol << std::endl;
        }

        // Close the dialog
        Close(true);
    }
}

void SVSelectionDialog::OnUpdate(wxCommandEvent &event)
{
    if (wxGetApp().network_interface.get_current_device() == "")
    {
        wxMessageBox(wxT("Network Interface is not selected!\n\nFirst, select Network Interface."), wxT("Error"), wxICON_ERROR);
    }
    else
    {
        auto sv_list = wxGetApp().sv_sub.get_sv_list();
    }
}