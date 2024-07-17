#include "drawing_panel.h"
#include "main.h"

DrawingPanel::DrawingPanel(wxWindow *parent)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL),
      isGreen(false)
{
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);
    this->Bind(wxEVT_MOTION, &DrawingPanel::OnMouseMotion, this);

    SetVirtualSize(14400, 200);
    SetScrollRate(1, 0);
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

    // Point "Signal Update"
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(0, 0, 100, 30);
    
    dc.DrawText(wxT("Update:"), wxPoint(5, 5));

    if (isGreen)
    {
        dc.SetPen(*wxWHITE_PEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(75, 14, 6);
    }
    else
    {
        dc.SetPen(*wxWHITE_PEN);
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawCircle(75, 14, 6);
    }

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
            // std::cout << "Drawing!!!" << std::endl;
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

            // Draw a vertical line at the cursor position
            dc.SetPen(*wxBLACK_DASHED_PEN);
            dc.DrawLine(m_cursorPosition.x, 0, m_cursorPosition.x, height);

            // Draw the cursor coordinates as a string
            wxString positionString = wxString::Format("(%d, %d)", (m_cursorPosition.x + offset.x), m_cursorPosition.y);
            dc.DrawText(positionString, m_cursorPosition.x + 15, m_cursorPosition.y + 15);
        }
    }
}

void DrawingPanel::OnMouseMotion(wxMouseEvent &event)
{
    // Get the cursor coordinates relative to the panel
    m_cursorPosition = event.GetPosition();
    Refresh();
}