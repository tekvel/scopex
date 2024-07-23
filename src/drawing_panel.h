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
#include "sv_subs_factory.h"

class DrawingPanel : public wxScrolledWindow
{
public:
    DrawingPanel(wxWindow *parent, int position);

    int pos;    // Position of DrawingPanel: 1 - down (Voltages), 0 - up (Currents)

    float xScale;
    float yScale;

    wxPoint pivotPoint;

    bool isGreen;

    wxPoint GetPivotPoint() const { return pivotPoint; }
    void SetPivotPoint(wxPoint pivot);

    float GetXScale() const { return xScale; }
    float GetYScale() const { return yScale; }
    void SetScale(float xScale, float yScale);

private:
    void OnPaint(wxPaintEvent &event);
    void Render(wxDC &dc);

    void OnMouseMotion(wxMouseEvent &event);
    void OnScroll(wxScrollWinEvent &event);
    void OnMouseWheel(wxMouseEvent &event);
    void OnMouseEnter(wxMouseEvent &event);
    void OnMouseLeave(wxMouseEvent &event);
    void OnMouseLeftDown(wxMouseEvent &event);
    void OnMouseLeftUp(wxMouseEvent &event);

    wxWindow *m_parent;
    wxPoint m_cursorPosition;
    bool cursorInside;
    bool isDragging;
    int lastMouseX;
};

#endif