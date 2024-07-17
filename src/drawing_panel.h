#ifndef DRAWING_PANEL_H
#define DRAWING_PANEL_H

#define PHASE_A_COLOR wxColor(0, 255, 0)
#define PHASE_B_COLOR wxColor(255, 0, 0)
#define PHASE_C_COLOR wxColor(0, 0, 255)

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/scrolwin.h>

#include "sv_handler_factory.h"
#include "sv_handler.h"

class DrawingPanel : public wxScrolledWindow
{
public:
    DrawingPanel(wxWindow *parent);

    float xScale;
    float yScale;

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
    wxPoint m_cursorPosition;
};

#endif