#ifndef SV_HANDLER_THREAD_H
#define SV_HANDLER_THREAD_H

#include <wx/thread.h>

#include "sv_subs_factory.h"
#include "network_interface.h"
#include "main_frame.h"

class SVHandlerThread : public wxThread
{
public:
    SVHandlerThread();
    virtual ~SVHandlerThread();

    virtual void *Entry() wxOVERRIDE;

    void Stop();

private:
};

class SVProcessThread : public wxThread
{
public:
    SVProcessThread();
    virtual ~SVProcessThread();

    virtual void *Entry() wxOVERRIDE;

private:
};

#endif