#include "drawing_panel.h"
#include "main.h"

DrawingPanel::DrawingPanel(wxWindow *parent, int position)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL),
      pos(position),
      isGreen(false),
      xScale(0.1), yScale(0.5),
      pivotPoint(0, 0)
{
    m_parent = this->GetParent();

    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Bind paint event
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);

    // Bind mouse motion event
    this->Bind(wxEVT_MOTION, &DrawingPanel::OnMouseMotion, this);

    // Bind mouse wheel event
    this->Bind(wxEVT_MOUSEWHEEL, &DrawingPanel::OnMouseWheel, this);

    // Bind scroll events
    this->Bind(wxEVT_SCROLLWIN_THUMBTRACK, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_LINEUP, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_LINEDOWN, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_PAGEUP, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_PAGEDOWN, &DrawingPanel::OnScroll, this);

    SetVirtualSize(14400, 200);
    SetScrollRate(1, 0);
}

void DrawingPanel::SetScale(float xScale, float yScale)
{
    this->xScale = xScale;
    this->yScale = yScale;

    Refresh();
}

void DrawingPanel::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);
    Render(dc);
}

void DrawingPanel::Render(wxDC &dc)
{
    dc.Clear();

    auto sv_indices = wxGetApp().sv_handler.GetListOfSVIndices();
    auto idx = wxGetApp().sv_sub.selectedSV_id_main;

    if (sv_indices.size() != 0 && idx != nullptr)
    {
        auto it = wxGetApp().sv_sub.sv_list->begin();
	    std::advance(it, *idx);

        auto number_of_points = it->F;
        auto number_of_meas = it->DatSet;

        wxSize size = this->GetSize();
        int width = size.GetWidth();
        int height = size.GetHeight();

        int virtualWidth = static_cast<int>(number_of_points + width);
        SetVirtualSize(virtualWidth, 200);

        // Scaling center point
        int centerX = width / 2;
        int centerY = height / 2;

        // Axes
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, centerY, width, centerY);
        dc.DrawLine(centerX, 0, centerX, height);

        // Display Point "Signal Update" if pos = 0 (up drawing panel)
        if (pos == 0)
        {
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
        }
        
        // Calculate the offset relative to the shift of wxScrolledWindow
        int offset_x = GetScrollPos(wxHORIZONTAL);

        std::vector<wxPoint> scaledPointsA;
        std::vector<wxPoint> scaledPointsB;
        std::vector<wxPoint> scaledPointsC;

        pivotPoint.x = centerX + offset_x;
        
        auto sv_handler_ptr = wxGetApp().sv_handler.GetSVHandler(*idx);

        if (!sv_handler_ptr->SV_data.empty())
        {
            // std::cout << "Drawing!!!" << std::endl;
            for (const auto &data : sv_handler_ptr->SV_data)
            {
                int x = static_cast<int>((data.smpCnt - pivotPoint.x + centerX) * xScale + pivotPoint.x) - offset_x;
                int y = static_cast<int>(centerY - (data.PhsMeasList[0 + number_of_meas/2 * pos].secData/10 * yScale));
                scaledPointsA.emplace_back(wxPoint(x, y));

                y = static_cast<int>(centerY - (data.PhsMeasList[1 + number_of_meas/2 * pos].secData/10 * yScale));
                scaledPointsB.emplace_back(wxPoint(x, y));

                y = static_cast<int>(centerY - (data.PhsMeasList[2 + number_of_meas/2 * pos].secData/10 * yScale));
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
            wxString positionString = wxString::Format("(%d, %d)", (m_cursorPosition.x + offset_x), m_cursorPosition.y);
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

void DrawingPanel::OnScroll(wxScrollWinEvent &event)
{
    int newPosition = GetScrollPos(wxHORIZONTAL);
    wxGetApp().GetMainFrame()->m_dp->SynchronizeScroll(this, newPosition);

    event.Skip();
}

void DrawingPanel::OnMouseWheel(wxMouseEvent &event)
{
    int rotation = event.GetWheelRotation();
    int linesPerRotation = event.GetLinesPerAction();
    int scrollAmount = rotation / event.GetWheelDelta() * linesPerRotation * 10;

    int newScrollPos = GetScrollPos(wxHORIZONTAL) - scrollAmount;

    SetScrollPos(wxHORIZONTAL, newScrollPos, true);
    wxGetApp().GetMainFrame()->m_dp->SynchronizeScroll(this, newScrollPos);

    Refresh();

    event.Skip();
}