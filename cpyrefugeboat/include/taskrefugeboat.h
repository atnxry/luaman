#ifndef __CPY_REFUGEBOAT_H__
#define __CPY_REFUGEBOAT_H__

#include "dm.h"

void TzRefugeboat_InitCriticalSection();
int api_TaskRefugeboatInit(Idmsoft* dmobj, void* ctxt);
int api_TaskRefugeboatMain(Idmsoft* dmobj, void* ctxt);
int api_TaskRefugeboatExit(Idmsoft* dmobj, void* ctxt);

#endif

