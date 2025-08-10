#ifndef __PARAM_H__
#define __PARAM_H__

#include "config.h"

class CParameters
{
public:
    struct cluamanconfig m_config;
    
public:
    CParameters();
    ~CParameters();

public:
    virtual int SetConfig(struct cluamanconfig* config);
    
    virtual struct cluamanconfig* GetConfig(){return &m_config;}
    virtual void SetSelectedFrontIndex(int selected_front_index);
    virtual void SetDirection(int direction);
    virtual void SetRole(int role);
    virtual void SetGroup(int grp);
    virtual void SetBlueMax(int bluemax);
    virtual void SetBlueMin(int bluemin);
    virtual void SetBlueThreshold(int bluethreshold);
    virtual void SetServer(char* server);
    virtual void SetPort(int port);
    virtual void SetHost(int host);
    virtual void SetLoopNum(unsigned int loopnum);
    virtual void SetPalyWndIndex(unsigned int playWndIndexVal);
    virtual void SetTransWait(int Wait2Sleep);
    virtual void SetExitGameCopyFull(int exitGameCopyFull);

    //ÐÂÔö
    virtual void SetCleanPackageTime(int cleanPackageTime);
    virtual void SetCleanPackageWhenStart(int cleanPackageWhenStart);
    virtual void SetCallBabyDie(int callbabydie);
    virtual void SetCleanPackage1(int cleanPackage1);
    virtual void SetCleanPackage2(int cleanPackage2);
    virtual void SetCleanPackage3(int cleanPackage3);
    virtual void SetCleanPackage4(int cleanPackage4);
    virtual void SetCleanPackage5(int cleanPackage5);
    virtual void SetCleanPackage(int cleanPackage);
    virtual void SetWaitCopytime(int waitCopytime);
    virtual void SetBluebloodChecktime(int bluebloodChecktime);
    virtual void SetBlockTimes(int blockTimes);
};


#endif
