#include "sinusoidal_panel.h"
#include "signals.h"
#include <string>
#include <vector>

SinusoidalPanel::SinusoidalPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL),
    m_cursorPos(0,0),
    xScale(1.0),yScale(1.0),pivotPoint(0,0)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &SinusoidalPanel::OnPaint, this);

    this->Bind(wxEVT_MOTION, &SinusoidalPanel::OnMouseMotion, this);

    Signals::getInstance().generateSignalPoints(32000, AMPLITUDE, FREQUENCY);
}

void SinusoidalPanel::SetScale(float xScale, float yScale){
    this->xScale = xScale;
    this->yScale = yScale;
    Refresh();
}

void SinusoidalPanel::SetPivotPoint(wxPoint pivot){
    this->pivotPoint = pivot;
    Refresh();
}

void SinusoidalPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    Render(dc);
}

void SinusoidalPanel::Render(wxDC& dc)
{
    dc.Clear();

    wxSize size = this->GetSize();
    int width = size.GetWidth();
    int height = size.GetHeight();

    // scaling center point
    int centerX = width / 2;
    int centerY = height / 2;

    // axes
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(0, centerY, width, centerY);
    dc.DrawLine(centerX, 0, centerX, height);

    // yTicks
    dc.DrawLine(centerX - 10, centerY - 100, centerX + 10, centerY - 100);
    dc.DrawLine(centerX - 10, centerY - 200, centerX + 10, centerY - 200);
    dc.DrawLine(centerX - 10, centerY + 100, centerX + 10, centerY + 100);
    dc.DrawLine(centerX - 10, centerY + 200, centerX + 10, centerY + 200);

    // xTicks
    // for (int i = 0; i < 5; ++i){
    //     dc.DrawLine(centerX + 200/multiplier*5*i, centerY - 10, centerX + 200/multiplier*5*i, centerY + 10);
    //     dc.DrawLine(centerX - 200/multiplier*5*i, centerY - 10, centerX - 200/multiplier*5*i, centerY + 10);
    // }

    std::vector<std::array<float, 2>>& points = Signals::getInstance().getData();
    int numPoints = static_cast<int>(points.size());

    std::vector<wxPoint> scaledPointsA;

    for (int i = 0; i != numPoints; ++i){
        int x = static_cast<int>((i - pivotPoint.x) * xScale + pivotPoint.x);
        // int x = static_cast<int>((points[i][0] - pivotPoint.x) * xScale + pivotPoint.x);
        int y = static_cast<int>(centerY - (points[i][1] - pivotPoint.y) * yScale + pivotPoint.y);
        scaledPointsA.emplace_back(wxPoint(x,y));
    }

    // Phase A
    dc.SetPen(wxPen(wxColor(PHASE_A_COLOR), 5));
    // dc.DrawSpline(actualPoints, points_A);
    dc.DrawSpline(scaledPointsA.size(), &scaledPointsA[0]);

    

    // Draw a vertical line at the cursor position
    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.DrawLine(m_cursorPos.x, 0, m_cursorPos.x, height);

    // Draw the cursor coordinates as a string
    wxString positionString = wxString::Format("(%d, %d)", m_cursorPos.x, m_cursorPos.y);
    dc.DrawText(positionString, m_cursorPos.x + 15, m_cursorPos.y + 15);

    wxPoint* found_point;
    for (int i = 0; i != numPoints; ++i){
        if (scaledPointsA[i].x == m_cursorPos.x){
            found_point = &scaledPointsA[i];
        }
    }
    wxString valueString = wxString::Format("(%d)", -(found_point->y - height/2));
    dc.DrawText(valueString, m_cursorPos.x + 15, m_cursorPos.y + 30);

    scaledPointsA.clear();
    scaledPointsA.shrink_to_fit();
}

void SinusoidalPanel::OnMouseMotion(wxMouseEvent& event)
{
    // Get the cursor coordinates relative to the panel
    m_cursorPos = event.GetPosition();
    
    // Refresh the panel to trigger a repaint
    Refresh();
}
