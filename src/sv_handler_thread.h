#ifndef SV_HANDLER_THREAD_H
#define SV_HANDLER_THREAD_H

#include "sv_subs_factory.h"
#include "network_interface.h"

class SVHandlerThread : public wxThread
{
public:
    SVHandlerThread();
    virtual ~SVHandlerThread();

    virtual void *Entry() wxOVERRIDE;

private:
};

#endif