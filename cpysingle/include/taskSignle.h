#ifndef __TASK_SIGNLE_H__
#define __TASK_SIGNLE_H__

#include "dm.h"
int api_TaskSignlePrepare(void);

int api_TaskSignleInit(Idmsoft* dmobj, void* ctxt);
int api_TaskSignleMain(Idmsoft* dmobj, void* ctxt);
int api_TaskSignleExit(Idmsoft* dmobj, void* ctxt);

#endif
