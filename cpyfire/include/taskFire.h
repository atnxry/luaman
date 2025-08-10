#ifndef __TASK_FIRE_H__
#define __TASK_FIRE_H__

#include "dm.h"

int api_TaskFireInit(Idmsoft* dmobj, void* ctxt);
int api_TaskFireMain(Idmsoft* dmobj, void* ctxt);
int api_TaskFireExit(Idmsoft* dmobj, void* ctxt);
void firecpy_InitCriticalSection();


#endif
