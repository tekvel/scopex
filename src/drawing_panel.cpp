#include "drawing_panel.h"
#include "main.h"

DrawingPanel::DrawingPanel(wxWindow *parent, int position)
    : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL),
      pos(position),
      isGreen(false),
      xScale(0.1), yScale(0.5),
      pivotPoint(80, 0),
      cursorInside(false),
      isDragging(false), lastMouseX(0)
{
    m_parent = this->GetParent();

    this->SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Bind paint event
    this->Bind(wxEVT_PAINT, &DrawingPanel::OnPaint, this);

    // Bind mouse events
    this->Bind(wxEVT_MOTION, &DrawingPanel::OnMouseMotion, this);
    this->Bind(wxEVT_MOUSEWHEEL, &DrawingPanel::OnMouseWheel, this);
    this->Bind(wxEVT_LEFT_DOWN, &DrawingPanel::OnMouseLeftDown, this);
    this->Bind(wxEVT_LEFT_UP, &DrawingPanel::OnMouseLeftUp, this);

    // Bind scroll events
    this->Bind(wxEVT_SCROLLWIN_THUMBTRACK, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_LINEUP, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_LINEDOWN, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_PAGEUP, &DrawingPanel::OnScroll, this);
    this->Bind(wxEVT_SCROLLWIN_PAGEDOWN, &DrawingPanel::OnScroll, this);

    // Bind mouse enter and leave events
    this->Bind(wxEVT_ENTER_WINDOW, &DrawingPanel::OnMouseEnter, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &DrawingPanel::OnMouseLeave, this);

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
        int vertical_axis_pos = 80;
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(0, centerY, width, centerY);
        dc.DrawLine(vertical_axis_pos, 0, vertical_axis_pos, height);

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

        pivotPoint.x = centerX;
        
        auto sv_handler_ptr = wxGetApp().sv_handler.GetSVHandler(*idx);

        std::vector<float> amp(2);
        amp[0] = sv_handler_ptr->SAnalyzer.current_amp;
        amp[1] = sv_handler_ptr->SAnalyzer.voltage_amp;

        float yWindowScale = 1;
        int x, y;

        if (!sv_handler_ptr->SV_data.empty())
        {
            for (const auto &data : sv_handler_ptr->SV_data)
            {
                yWindowScale = height / amp[pos] / 2;

                x = static_cast<int>((data.smpCnt - offset_x) * xScale) + pivotPoint.x;
                // x = static_cast<int>((data.smpCnt - pivotPoint.x + centerX) * xScale + pivotPoint.x) - offset_x;
                y = static_cast<int>(centerY - (data.PhsMeasList[0 + number_of_meas/2 * pos].secData * yScale) * yWindowScale);
                scaledPointsA.emplace_back(wxPoint(x, y));

                y = static_cast<int>(centerY - (data.PhsMeasList[1 + number_of_meas/2 * pos].secData * yScale) * yWindowScale);
                scaledPointsB.emplace_back(wxPoint(x, y));

                y = static_cast<int>(centerY - (data.PhsMeasList[2 + number_of_meas/2 * pos].secData * yScale) * yWindowScale);
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

            // yTicks
            dc.SetPen(*wxBLACK_PEN);
            for (int i = 1; i != 3; ++i)
            {
                dc.DrawLine(vertical_axis_pos - 10, centerY - amp[pos] / 2 * i * yScale * yWindowScale, vertical_axis_pos + 10, centerY - amp[pos] / 2 * i * yScale * yWindowScale);
                wxString amp_string = wxString::Format("%.1f", (amp[pos] / 2 * i));
                dc.DrawText(amp_string, vertical_axis_pos - 60, centerY - amp[pos] / 2 * i * yScale * yWindowScale);

                dc.DrawLine(vertical_axis_pos - 10, centerY + amp[pos] / 2 * i * yScale * yWindowScale, vertical_axis_pos + 10, centerY + amp[pos] / 2 * i * yScale * yWindowScale);
                amp_string = wxString::Format("%.1f", -(amp[pos] / 2 * i));
                dc.DrawText(amp_string, vertical_axis_pos - 60, centerY + amp[pos] / 2 * i * yScale * yWindowScale);
            }

            // Delete scaledPoints variable
            scaledPointsA.clear();
            scaledPointsA.shrink_to_fit();
            scaledPointsB.clear();
            scaledPointsB.shrink_to_fit();
            scaledPointsC.clear();
            scaledPointsC.shrink_to_fit();

            if (cursorInside)
            {
                // Draw a vertical line at the cursor position
                dc.SetPen(*wxBLACK_DASHED_PEN);
                dc.DrawLine(m_cursorPosition.x, 0, m_cursorPosition.x, height);

                // Draw the cursor coordinates as a string
                wxString positionString = wxString::Format("(%d, %d)", (m_cursorPosition.x + offset_x), m_cursorPosition.y);
                dc.DrawText(positionString, m_cursorPosition.x + 15, m_cursorPosition.y + 15);
            }
        }
    }
}

void DrawingPanel::OnMouseMotion(wxMouseEvent &event)
{
    // Get the cursor coordinates relative to the panel
    m_cursorPosition = event.GetPosition();

    if (isDragging)
    {
        int deltaX = (event.GetX() - lastMouseX) * 5;
        int newScrollPos = GetScrollPos(wxHORIZONTAL) - deltaX;

        SetScrollPos(wxHORIZONTAL, newScrollPos, true);
        wxGetApp().GetMainFrame()->m_dp->SynchronizeScroll(this, newScrollPos);
    }

    lastMouseX = event.GetX();
    Refresh();
    event.Skip();
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

    float scaleFactor = 0.05f;
    if (rotation > 0)
    {
        xScale += scaleFactor;
    }
    else if (rotation < 0)
    {
        xScale = std::max(0.1f, xScale - scaleFactor); // Ensure xScale does not go below a minimum value
    }

    wxGetApp().GetMainFrame()->m_dp->SynchronizeXScale(this, xScale);

    Refresh();

    event.Skip();
}

void DrawingPanel::OnMouseEnter(wxMouseEvent &event)
{
    cursorInside = true;
    Refresh();
    event.Skip();
}

void DrawingPanel::OnMouseLeave(wxMouseEvent &event)
{
    cursorInside = false;
    Refresh();
    event.Skip();
}

void DrawingPanel::OnMouseLeftDown(wxMouseEvent &event)
{
    if (event.LeftDown())
    {
        isDragging = true;
        SetCursor(wxCursor(wxCURSOR_HAND));
        lastMouseX = event.GetX();
        pivotPoint.x = m_cursorPosition.x;
    }
    event.Skip();
}

void DrawingPanel::OnMouseLeftUp(wxMouseEvent &event)
{
    if (isDragging)
    {
        isDragging = false;
        SetCursor(wxCursor(wxCURSOR_ARROW));
    }
    event.Skip();
}