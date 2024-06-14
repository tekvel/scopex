#include "sinusoidal_panel.h"
#include "signals.h"
#include <string>
#include <vector>

SinusoidalPanel::SinusoidalPanel(wxWindow* parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL),
    m_cursorPos(0,0),
    xScale(1.0),yScale(0.5),pivotPoint(0,0),lcnt(0)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    this->Bind(wxEVT_PAINT, &SinusoidalPanel::OnPaint, this);

    this->Bind(wxEVT_MOTION, &SinusoidalPanel::OnMouseMotion, this);

    // Signals::getInstance().generateSignalPoints(AMPLITUDE, FREQUENCY, 0.0f);
    Signals::getInstance().addSignal(AMPLITUDE, FREQUENCY, 0.0f);
    Signals::getInstance().addSignal(AMPLITUDE, FREQUENCY, PHASESHIFT120);
    Signals::getInstance().addSignal(AMPLITUDE, FREQUENCY, PHASESHIFTNEG120);

    SetVirtualSize(Signals::getInstance().getSignalLength(), 200);
    SetScrollRate(1,0);
}

void SinusoidalPanel::SetScale(float xScale, float yScale){
    this->xScale = xScale;
    this->yScale = yScale;

    int virtualWidth = static_cast<int>(Signals::getInstance().getSignalLength() * xScale);
    SetVirtualSize(virtualWidth, 400);

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
    for (int i = 0; i < 5; ++i){
        dc.DrawLine(centerX + 200/xScale*5*i, centerY - 10, centerX + 200/xScale*5*i, centerY + 10);
        dc.DrawLine(centerX - 200/xScale*5*i, centerY - 10, centerX - 200/xScale*5*i, centerY + 10);
    }

    std::vector<Instance>& data_A = Signals::getInstance().getData(0);
    std::vector<Instance>& data_B = Signals::getInstance().getData(1);
    std::vector<Instance>& data_C = Signals::getInstance().getData(2);
    int numPoints = static_cast<int>(data_A.size());

    std::vector<wxPoint> scaledPointsA;
    std::vector<wxPoint> scaledPointsB;
    std::vector<wxPoint> scaledPointsC;
    
    wxPoint offset = GetViewStart();      // Calculate the offset relative to the shift of wxScrolledWindow

    for (int i = 0; i != numPoints; ++i){
        int x = static_cast<int>((data_A[i].timeStamp.utcTime.sec-pivotPoint.x) * xScale + pivotPoint.x) - offset.x;
        int y = static_cast<int>(centerY - (data_A[i].val.secData * yScale));
        scaledPointsA.emplace_back(wxPoint(x,y));
        x = static_cast<int>((data_B[i].timeStamp.utcTime.sec-pivotPoint.x) * xScale + pivotPoint.x) - offset.x;
        y = static_cast<int>(centerY - (data_B[i].val.secData * yScale));
        scaledPointsB.emplace_back(wxPoint(x,y));
        x = static_cast<int>((data_C[i].timeStamp.utcTime.sec-pivotPoint.x) * xScale + pivotPoint.x) - offset.x;
        y = static_cast<int>(centerY - (data_C[i].val.secData * yScale));
        scaledPointsC.emplace_back(wxPoint(x,y));
    }

    // Phase A
    dc.SetPen(wxPen(wxColor(PHASE_A_COLOR), 5));
    dc.DrawSpline(scaledPointsA.size(), &scaledPointsA[0]);
    // Phase B
    dc.SetPen(wxPen(wxColor(PHASE_B_COLOR), 5));
    dc.DrawSpline(scaledPointsA.size(), &scaledPointsB[0]);
    // Phase C
    dc.SetPen(wxPen(wxColor(PHASE_C_COLOR), 5));
    dc.DrawSpline(scaledPointsA.size(), &scaledPointsC[0]);

    // if (!scaledPointsA.empty()){
    //     for (int i = 0; i != numPoints; ++i){
    //         if (scaledPointsA[i].x == 1000){
    //             lcnt = i;
    //         }
    //     }
    // }
    // dc.DrawText(wxString::Format("(%d)", lcnt), wxPoint(100, 100));
    // dc.DrawText(wxString::Format("(%d)", xScale), wxPoint(200, 220));

    

    // Draw a vertical line at the cursor position
    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.DrawLine(m_cursorPos.x, 0, m_cursorPos.x, height);

    // Draw the cursor coordinates as a string
    wxString positionString = wxString::Format("(%d, %d)", (m_cursorPos.x + offset.x), m_cursorPos.y);
    dc.DrawText(positionString, m_cursorPos.x + 15, m_cursorPos.y + 15);

    // Draw the value of signal as a string 
    wxPoint* valueA = nullptr;
    wxPoint* valueB = nullptr;
    wxPoint* valueC = nullptr;
    for (int i = 0; i != numPoints; ++i){
        if (scaledPointsA[i].x == m_cursorPos.x){
            valueA = &scaledPointsA[i];
            valueB = &scaledPointsB[i];
            valueC = &scaledPointsC[i];
        }
    }
    if (valueA && valueB && valueC) {
        wxString valueString = wxString::Format("(%d, %d, %d)", -(valueA->y - centerY), -(valueB->y - centerY), -(valueC->y - centerY));
        dc.DrawText(valueString, m_cursorPos.x + 15, m_cursorPos.y + 30);
    }

    // Delete scaledPoints variable
    scaledPointsA.clear();
    scaledPointsA.shrink_to_fit();
    scaledPointsB.clear();
    scaledPointsB.shrink_to_fit();
    scaledPointsC.clear();
    scaledPointsC.shrink_to_fit();
}

void SinusoidalPanel::OnMouseMotion(wxMouseEvent& event)
{
    // Get the cursor coordinates relative to the panel
    m_cursorPos = event.GetPosition();
    Refresh();
}
