#ifndef __TASK_EXSIGNLE_H__
#define __TASK_EXSIGNLE_H__

#include "dm.h"
int api_TaskExSignlePrepare(void);

int api_TaskExSignleInit(Idmsoft* dmobj, void* ctxt);
int api_TaskExSignleMain(Idmsoft* dmobj, void* ctxt);
int api_TaskExSignleExit(Idmsoft* dmobj, void* ctxt);

#endif
