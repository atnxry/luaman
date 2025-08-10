#ifndef __KEYBOARD_MONITOR_H__
#define __KEYBOARD_MONITOR_H__
#include "threadctrl.h"
#include "dm.h"

typedef struct tagKEYBOARD_MONITOR_S
{
    HANDLE lock;
    unsigned int ref;
    unsigned char stop;
    THREAD_S* thread;
    int (*callback)(Idmsoft* dmobj, void* ctxt);
    void* context;
}KEYBOARD_MONITOR_S;

class CKeyboardMonitor
{
public:
    CKeyboardMonitor();
    ~CKeyboardMonitor();

public:
    void set(int (*callback)(Idmsoft* dmobj, void* ctxt), void* context);
    int run();
    void kill();
    
public:
    KEYBOARD_MONITOR_S keyboard_monitor;
};

int keyboard_monitor_callback(Idmsoft* dmobj, void* ctxt);

#endif

