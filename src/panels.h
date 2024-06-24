#ifndef PANELS_H
#define PANELS_H

#include <wx/wx.h>
#include <wx/panel.h>
#include "sinusoidal_panel.h"
#include "vector_panel.h"

class upPanel : public wxPanel
{
public:
    upPanel(wxPanel *parent);

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
};

class downPanel : public wxPanel
{
public:
    downPanel(wxPanel *parent);
    VectorPanel *m_vecPanel;
    SinusoidalPanel *m_sinPanel;

private:
    wxBoxSizer *m_bsizer2;
};

const int ID_X = 101;
const int ID_Y = 102;

#endif // PANELS_H
