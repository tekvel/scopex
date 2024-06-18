#include "main.h"
#include "communicate.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    Communicate *communicate = new Communicate(nullptr, wxID_ANY, wxT("3PhaseDiagrams"));
    communicate->Show(true);

    return true;
}
