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

#include "vector_panel.h"

VectorPanel::VectorPanel(wxWindow *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &VectorPanel::OnPaint, this);
}

void VectorPanel::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    Render(dc);
}

void VectorPanel::Render(wxDC &dc)
{
    dc.Clear();

    wxSize size = this->GetSize();
    int width = size.GetWidth();
    int height = size.GetHeight();

    // Origin center point
    int centerX = width / 2;
    int centerY = height / 2;

    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.DrawCircle(centerX, centerY, width / 2 * 0.8);

    // Axes
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(0, centerY, width, centerY);
    dc.DrawLine(centerX, 0, centerX, height);

    // dc.DrawText(wxString::Format("(%d)", (width)), centerX, centerY-40);

    // Drawing vector for sinusoidal signal
    double amplitude = 200.0;

    dc.SetPen(wxPen(wxColor(PHASE_A_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude / AMPLITUDE * width * 0.45 * cos(0.0f), centerY - amplitude / AMPLITUDE * width * 0.45 * sin(0.0f));
    dc.SetPen(wxPen(wxColor(PHASE_B_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude / AMPLITUDE * width * 0.45 * cos(PHASESHIFT120), centerY - amplitude / AMPLITUDE * width * 0.45 * sin(PHASESHIFT120));
    dc.SetPen(wxPen(wxColor(PHASE_C_COLOR), 5));
    dc.DrawLine(centerX, centerY, centerX + amplitude / AMPLITUDE * width * 0.45 * cos(PHASESHIFTNEG120), centerY - amplitude / AMPLITUDE * width * 0.45 * sin(PHASESHIFTNEG120));
}
