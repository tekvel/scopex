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
