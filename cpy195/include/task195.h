#ifndef __TASK195_H__
#define __TASK195_H__
#include "dm.h"

int api_Task195Main(Idmsoft* dmobj, void* ctxt);
int api_Task195Init(Idmsoft* dmobj, void* ctxt);
int api_Task195Exit(Idmsoft* dmobj, void* ctxt);
void Tz195_InitCriticalSection(void);

#define MAP_SEGRILA      (1)
#define MAP_FORGET_TOWER (2)

//ÄÚ²¿º¯Êý
int Tz195_CheckBlueOut(Idmsoft* dmobj, void* ctxt);
int Tz195_CheckBlue(Idmsoft* dmobj, void* ctxt);
int Tz195_InviteHeilfellow(Idmsoft* dmobj, void* ctxt, int syncCountMax);
int Tz195_MovePosition(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs);
void Tz195_ReleaseKey(Idmsoft* dmobj, void* ctxt, struct luamanwalkstate* walkstate);
int Tz195_CheckMapStyle(Idmsoft* dmobj, void* ctxt);
int Tz195_CheckLefttime2Terminate(Idmsoft* dmobj, void* ctxt);
int Tz195_Login(Idmsoft* dmobj, void* ctxt);
int Tz195_FightatFixPointSimple(Idmsoft* dmobj, void* ctxt, int location, char* fposXY);
int Tz195_CheckFightSkip(Idmsoft* dmobj, void* ctxt, int location);
int Tz195_FightAutoStart(Idmsoft* dmobj, void* ctxt);
int Tz195_FightAutoStop(Idmsoft* dmobj, void* ctxt);
int Tz195_FightatFixPoint(Idmsoft* dmobj, void* ctxt, int location, char* fposXY);
int Tz195_SlaveMainLine_Continue(Idmsoft* dmobj, void* ctxt);
int Tz195_MainAuxi(Idmsoft* dmobj, void* ctxt);
int Tz195_Walk2WaitPoint(Idmsoft* dmobj, void* ctxt);


#endif
