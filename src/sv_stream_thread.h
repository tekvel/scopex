#ifndef SV_STREAM_THREAD_H
#define SV_STREAM_THREAD_H

#include <wx/wx.h>
#include <wx/thread.h>

#include <memory>

#include "network_interface.h"
#include "sv_dialog.h"
#include "sv_subs_factory.h"

class SVSearchThread : public wxThread
{
public:
    SVSearchThread();
    virtual ~SVSearchThread();

    virtual void *Entry() wxOVERRIDE;

    void Stop();

private:
};

#endif