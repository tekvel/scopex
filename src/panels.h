#ifndef PANELS_H
#define PANELS_H

#include <wx/wx.h>
#include <wx/panel.h>
#include "sinusoidal_panel.h"
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
    VectorPanel *m_vecPanel;
    SinusoidalPanel *m_sinPanel1;
    SinusoidalPanel *m_sinPanel2;
    DrawingPanel *m_drawingPanel;
    wxPanel *m_leftPanel;
    wxPanel *m_rightPanel;

private:
    int num_of_sinPanels;
};

const int ID_X = 101;
const int ID_Y = 102;

#endif // PANELS_H
