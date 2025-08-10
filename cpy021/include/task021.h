#ifndef __TASK021_H__
#define __TASK021_H__

#include "dm.h"

struct fight021_param
{
    char* fposXY;
    char* ufontName;
    char* uName;
    char* uclrOffset;
    unsigned int uFailedCountMax;
};

int api_Task021Main(Idmsoft* dmobj, void* ctxt);
int api_Task021Init(Idmsoft* dmobj, void* ctxt);
int api_Task021Exit(Idmsoft* dmobj, void* ctxt);

int Tz021_FinishTask(Idmsoft* dmobj, void* ctxt);
int Tz021_FinishTask_slave(Idmsoft* dmobj, void* ctxt);
int Tz021_CheckLife(Idmsoft* dmobj, void* ctxt);
int Tz021_CheckLife_Outside(Idmsoft* dmobj, void* ctxt);
int Tz021_CheckAlive(Idmsoft* dmobj, void* ctxt);
int Tz021_Fly2Copy(Idmsoft* dmobj, void* ctxt);
int Tz021_Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir);
int Tz021_FightAutoStart(Idmsoft* dmobj, void* ctxt);
int Tz021_FightAutoStop(Idmsoft* dmobj, void* ctxt);
int Tz021_FightInFixPoint_atc(Idmsoft* dmobj, void* ctxt, struct fight021_param* params);


#endif
