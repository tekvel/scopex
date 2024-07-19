#ifndef PANELS_H
#define PANELS_H

#include <wx/wx.h>
#include <wx/panel.h>
#include "vector_panel.h"
#include "drawing_panel.h"

class upPanel : public wxPanel
{
public:
    upPanel(wxPanel *parent, int num_of_drawingPanels);

    void OnXScaleChanged(wxCommandEvent &event);
    void OnYScaleChanged(wxCommandEvent &event);
    void OnPivotXChanged(wxCommandEvent &event);
    void OnPivotYChanged(wxCommandEvent &event);

private:
    wxStaticText *m_textX;
    wxStaticText *m_textY;
    wxSlider *m_sliderX;
    wxSlider *m_sliderY;
    wxGridSizer *m_gsizer1;
    wxPanel *m_parent;
    float amp;
    int num_of_sinPanels;
};

class downPanel : public wxPanel
{
public:
    downPanel(wxPanel *parent, int num_of_drawingPanels);

    void SynchronizeScroll(DrawingPanel* source, int newPosition);

    VectorPanel *m_vecPanel;
    DrawingPanel *m_drawingPanel1;
    DrawingPanel *m_drawingPanel2;
    wxPanel *m_leftPanel;
    wxPanel *m_rightPanel;

private:
    int num_of_sinPanels;
};

const int ID_X = 101;
const int ID_Y = 102;

#endif // PANELS_H
