#ifndef OSC_PANEL_H
#define OSC_PANEL_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/notebook.h>

#include "vector_panel.h"
#include "drawing_panel.h"

class OscilloscopePanel : public wxPanel
{
public:
    OscilloscopePanel(wxNotebook *parent, int num_of_drawingPanels);

    void SynchronizeScroll(DrawingPanel *source, int newPosition);
    void SynchronizeXScale(DrawingPanel *source, float newXScale);

    VectorPanel *m_vecPanel;
    wxStaticText *m_text1;
    wxStaticText *m_text2;
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
