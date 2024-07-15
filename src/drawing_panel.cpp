#include "drawing_panel.h"
#include "main.h"

DrawingPanel::DrawingPanel(wxWindow *parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    // this->Bind(wxEVT_MOTION, &DrawingPanel::OnMouseMotion, this);

    SetVirtualSize(14400, 200);
    SetScrollRate(1, 0);

    // this->Unbind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
}

void DrawingPanel::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    Render(dc);
}

void DrawingPanel::Render(wxDC &dc)
{
    dc.Clear();

    wxSize size = this->GetSize();
    int width = size.GetWidth();
    int height = size.GetHeight();

    // Scaling center point
    int centerX = width / 2;
    int centerY = height / 2;

    // Axes
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawLine(0, centerY, width, centerY);
    dc.DrawLine(centerX, 0, centerX, height);

    // Calculate the offset relative to the shift of wxScrolledWindow
    wxPoint offset = GetViewStart();

    std::vector<wxPoint> scaledPointsA;
    std::vector<wxPoint> scaledPointsB;
    std::vector<wxPoint> scaledPointsC;

    pivotPoint.x = centerX + offset.x;

    auto sv_indices = wxGetApp().sv_handler.GetListOfSVIndices();

    if (sv_indices.size() != 0)
    {
        
        auto sv_handler_ptr = wxGetApp().sv_handler.GetSVHandler(sv_indices[0]);

        if (!sv_handler_ptr->SV_data.empty())
        {
            for (const auto &data : sv_handler_ptr->SV_data)
            {
                int x = static_cast<int>(data.smpCnt) - offset.x;
                int y = static_cast<int>(centerY - data.PhsMeasList[0].secData/10);
                scaledPointsA.emplace_back(wxPoint(x, y));
                x = static_cast<int>(data.smpCnt) - offset.x;
                y = static_cast<int>(centerY - data.PhsMeasList[1].secData/10);
                scaledPointsB.emplace_back(wxPoint(x, y));
                x = static_cast<int>(data.smpCnt) - offset.x;
                y = static_cast<int>(centerY - data.PhsMeasList[2].secData/10);
                scaledPointsC.emplace_back(wxPoint(x, y));
                
            }

            // Phase A
            dc.SetPen(wxPen(wxColor(PHASE_A_COLOR), 5));
            dc.DrawSpline(scaledPointsA.size(), &scaledPointsA[0]);
            // Phase B
            dc.SetPen(wxPen(wxColor(PHASE_B_COLOR), 5));
            dc.DrawSpline(scaledPointsB.size(), &scaledPointsB[0]);
            // Phase C
            dc.SetPen(wxPen(wxColor(PHASE_C_COLOR), 5));
            dc.DrawSpline(scaledPointsC.size(), &scaledPointsC[0]);

            // Delete scaledPoints variable
            scaledPointsA.clear();
            scaledPointsA.shrink_to_fit();
            scaledPointsB.clear();
            scaledPointsB.shrink_to_fit();
            scaledPointsC.clear();
            scaledPointsC.shrink_to_fit();
        }
    }
}