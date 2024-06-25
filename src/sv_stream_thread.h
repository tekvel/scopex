#ifndef SV_STREAM_THREAD_H
#define SV_STREAM_THREAD_H

#include <wx/wx.h>
#include <wx/thread.h>

#include "network_interface.h"

class SVSearchThread : public wxThread
{
public:
    SVSearchThread();
    virtual ~SVSearchThread();

    virtual void *Entry() wxOVERRIDE;

private:
};

#endif