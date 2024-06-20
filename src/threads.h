#ifndef THREADS_H
#define THREADS_H

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/app.h>

class MyThread : public wxThread
{
public:
    MyThread();
    virtual ~MyThread();

    virtual void *Entry() wxOVERRIDE;

public:
    unsigned m_count;
};
#endif