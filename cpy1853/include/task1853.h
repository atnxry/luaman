#ifndef __CPY1853_H__
#define __CPY1853_H__

#include "dm.h"

int api_Task1853Main(Idmsoft* dmobj, void* ctxt);
int api_Task1853Init(Idmsoft* dmobj, void* ctxt);
int api_Task1853Exit(Idmsoft* dmobj, void* ctxt);

int Tz185_PassCopyLoop(Idmsoft* dmobj, void* ctxt);
int Tz1853_CheckLife(Idmsoft* dmobj, void* ctxt);
int Tz1853_CheckLife_Outside(Idmsoft* dmobj, void* ctxt);
int Tz1853_CheckAlive(Idmsoft* dmobj, void* ctxt, int bCopyoutPre);
int Tz1853_Fly2Copy(Idmsoft* dmobj, void* ctxt);
void Tz1853_ReleaseKey(Idmsoft* dmobj, void* ctxt);
int Tz1853_InviteHeilfellowJoin2Team(Idmsoft* dmobj, void* ctxt);
void Tz1853_InitCriticalSection();


#endif
