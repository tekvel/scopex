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

#ifndef VECTOR_PANEL_H
#define VECTOR_PANEL_H

#define PHASE_A_COLOR wxColor(0, 255, 0)
#define PHASE_B_COLOR wxColor(255, 0, 0)
#define PHASE_C_COLOR wxColor(0, 0, 255)
#define AMPLITUDE 200.0f
#define FREQUENCY (50.0f / 1.0f)
#define PHASESHIFT120 (2.0f * M_PI / 3.0f)
#define PHASESHIFTNEG120 (-2.0f * M_PI / 3.0f)

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>

class VectorPanel : public wxPanel
{
public:
    VectorPanel(wxWindow *parent);

private:
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC &dc);
};

#endif
