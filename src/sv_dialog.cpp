/*
 * ScopeX
 * Copyright (C) 2024 Tekvel R&D Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "sv_dialog.h"
#include "main.h"

SVSelectionDialog::SVSelectionDialog(wxWindow *parent, const wxString &title)
    : wxDialog(parent, -1, title, wxDefaultPosition, wxSize(720, 350), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
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
    m_buttonUpdate->SetMaxSize( wxSize( 120,50 ) );
    hbox->Add(m_buttonUpdate, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    m_panel1->SetSizer(hbox);
    m_panel1->Layout();
    hbox->Fit(m_panel1);
    vbox2->Add(m_panel1, 1, wxEXPAND | wxALL, 5);

    m_svStreamList = new SVStreamList(m_upPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
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

    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnUpdate, this, wxID_SV_DIALOG_UPDATE);             // EVT_UPDATE_SV_DIALOG
    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnCancel, this, wxID_SV_DIALOG_CANCEL);             // EVT_CANCEL_SV_DIALOG
    Bind(wxEVT_BUTTON, &SVSelectionDialog::OnOK, this, wxID_SV_DIALOG_OK);                     // EVT_OK_SV_DIALOG
    Bind(wxEVT_THREAD, &SVSelectionDialog::OnSearchComplete, this, wxID_EVT_SEARCH_COMPLETED); // EVT_SV_SEARCH_IS_COMPLETED

    Centre();

    if (wxGetApp().sv_sub.sv_list->size() != 0)
    {
        UpdateSVList();
    }
}

SVSelectionDialog::~SVSelectionDialog()
{
    if (wxGetApp().network_interface.isCapturing)
    {
        wxGetApp().network_interface.stop_capture();
        wxGetApp().sv_sub.search_thread = nullptr;
    }
}

void SVSelectionDialog::OnCancel(wxCommandEvent &event)
{
    // Close the dialog
    Close(true);
}

void SVSelectionDialog::OnOK(wxCommandEvent &event)
{
    if (!wxGetApp().sv_sub.selectedSV_ids->empty())
    {
        wxGetApp().sv_sub.selectedSV_ids->clear();
    }
    wxArrayInt selectedItems;
    long item = -1;
    while ((item = m_svStreamList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND)
    {
        selectedItems.Add(item);
        wxGetApp().sv_sub.selectedSV_ids->push_back(item);
    }

    int n = selectedItems.GetCount();
    if (n == 0)
    {
        wxMessageBox(wxT("SV Stream is not selected!"), wxT("Error!"), wxICON_ERROR);
    }
    else
    {
        std::cout << "Number of selected items: " << n << std::endl;

        long num_of_drawingPanels = -1;

        wxGetApp().GetMainFrame()->toolComboBox->Clear();

        wxGetApp().sv_handler.DeleteHandlers();

        for (const auto &index : selectedItems)
        {
            wxString APPID = m_svStreamList->GetItemText(index, 2);
            wxString SVID = m_svStreamList->GetItemText(index, 3);
            wxString DatSet = m_svStreamList->GetItemText(index, 6);
            wxString SV_Stream = "APPID: " + APPID + "; SVID: " +  SVID;

            if (num_of_drawingPanels < 0)  // check if num_of_drawingPanels variable was changed or not
            {
                if (DatSet == "8" || DatSet == "6")
                {
                    num_of_drawingPanels = 2;
                }
                else
                {
                    num_of_drawingPanels = 1;
                }
            }

            wxGetApp().GetMainFrame()->toolComboBox->Append(SV_Stream);

            wxGetApp().sv_handler.CreateHandler(index);
        }
        // Get id of selected SV in ComboBox 
        auto id = wxGetApp().sv_sub.selectedSV_ids->at(0);
        std::shared_ptr<long> selectedSV_id_main = std::make_shared<long>(id);
        wxGetApp().sv_sub.selectedSV_id_main = selectedSV_id_main;  // Memorize id of SV selected in ComboBox
        wxGetApp().sv_sub.create_bpf_filter();                      // Create bpf filter for SV selected in ComboBox

        wxGetApp().GetMainFrame()->toolComboBox->SetSelection(0);
        wxGetApp().GetMainFrame()->num_of_drawingPanels = num_of_drawingPanels;
        wxGetApp().GetMainFrame()->RefreshPanels();
        wxGetApp().GetMainFrame()->EnableTools(true);

        // Close the dialog
        Close(true);
    }
}

void SVSelectionDialog::OnUpdate(wxCommandEvent &event)
{
    if (wxGetApp().network_interface.get_current_device() == "")
    {
        wxMessageBox(wxT("Network Interface is not selected!\n\nFirst, select Network Interface."), wxT("Error"), wxICON_ERROR);
        return;
    }
    m_buttonUpdate->Enable(false);
    if (wxGetApp().sv_sub.get_sv_list() == nullptr)
    {
        std::cerr << "Problem in starting SV Searching Thread!" << std::endl;
        return;
    }
}

void SVSelectionDialog::OnSearchComplete(wxThreadEvent &event)
{
    m_buttonUpdate->Enable(true);
    UpdateSVList();
}

void SVSelectionDialog::UpdateSVList()
{
    // Clear Current Items
    m_svStreamList->DeleteAllItems();

    auto sv_list = wxGetApp().sv_sub.sv_list;

    if (sv_list && sv_list->empty())
    {
        wxMessageBox(wxT("SV streams are not found!\n\nTry again."), wxT("Warning"), wxICON_WARNING);
        return;
    }

    for (const auto &stream : *sv_list)
    {
        wxString source;
        wxString destination;

        for (int i = 0; i < ETHER_ADDR_LEN; ++i)
        {
            source += wxString::Format("%02x", stream.ether_shost[i]);
            if (i < ETHER_ADDR_LEN - 1)
                source += ":";
            destination += wxString::Format("%02x", stream.ether_dhost[i]);
            if (i < ETHER_ADDR_LEN - 1)
                destination += ":";
        }
        
        wxString APPID = wxString::Format("0x%04x", stream.APPID);
        std::string svIDStr(stream.svID.begin(), stream.svID.end());
        wxString svID = wxString::FromUTF8(svIDStr.c_str());
        wxString noASDU = wxString::Format("%i", stream.noASDU);
        wxString F = wxString::Format("%llu", stream.F);
        wxString DatSet = wxString::Format("%i", stream.DatSet);

        m_svStreamList->AddStream(source, destination, APPID, svID, noASDU, F, DatSet);
    }
}