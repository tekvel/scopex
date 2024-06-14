#include "vector_panel.h"
#include "signals.h"

VectorPanel::VectorPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &VectorPanel::OnPaint, this);
}

void VectorPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    Render(dc);
}

void VectorPanel::Render(wxDC& dc)
{
    dc.Clear();

    wxSize size = this->GetSize();
    int width = size.GetWidth();
    int height = size.GetHeight();

    // Origin center point
    int centerX = width / 2;
    int centerY = height / 2;

    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.DrawCircle(centerX, centerY, width/2*0.8);

    // Axes
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(0, centerY, width, centerY);
    dc.DrawLine(centerX, 0, centerX, height);

    dc.DrawText(wxString::Format("(%d)", (width)), centerX, centerY-40);

    // Drawing vector for sinusoidal signal
    double amplitude = 100.0;
    
    dc.SetPen(wxPen(wxColor(PHASE_A_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude/AMPLITUDE * width * 0.9 * cos(0.0f), centerY - amplitude/AMPLITUDE * width * 0.9 * sin(0.0f));
    dc.SetPen(wxPen(wxColor(PHASE_B_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude/AMPLITUDE * width * 0.9 * cos(PHASESHIFT120), centerY - amplitude/AMPLITUDE * width * 0.9 * sin(PHASESHIFT120));
    dc.SetPen(wxPen(wxColor(PHASE_C_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude/AMPLITUDE * width * 0.9 * cos(PHASESHIFTNEG120), centerY - amplitude/AMPLITUDE * width * 0.9 * sin(PHASESHIFTNEG120));
}
