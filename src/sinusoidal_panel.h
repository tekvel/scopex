#ifndef SINUSOIDAL_PANEL_H
#define SINUSOIDAL_PANEL_H

#define PHASE_A_COLOR wxColor(0, 255, 0)
#define PHASE_B_COLOR wxColor(255, 0, 0)
#define PHASE_C_COLOR wxColor(0, 0, 255)

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/scrolwin.h>

class SinusoidalPanel : public wxScrolledWindow
{
public:
    SinusoidalPanel(wxWindow *parent);

    float xScale;
    float yScale;
    int lcnt;
    wxPoint pivotPoint;

    wxPoint GetPivotPoint() const { return pivotPoint; }
    void SetPivotPoint(wxPoint pivot);

    float GetXScale() const { return xScale; }
    float GetYScale() const { return yScale; }
    void SetScale(float xScale, float yScale);

private:
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC &dc);

    void OnMouseMotion(wxMouseEvent &event);
    wxPoint m_cursorPos;
};

#endif
