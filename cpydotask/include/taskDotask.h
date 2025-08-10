#ifndef __TASK_DOTASK_H__
#define __TASK_DOTASK_H__

#include "dm.h"

int api_TaskDotaskPrepare(void);
int api_TaskDotaskMain(Idmsoft* dmobj, void* ctxt);
int api_TaskDotaskInit(Idmsoft* dmobj, void* ctxt);
int api_TaskDotaskExit(Idmsoft* dmobj, void* ctxt);

#endif

