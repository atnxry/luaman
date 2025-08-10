#include <math.h>
#include <stdlib.h>
#include <process.h>
#include "stdafx.h"
//#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "taskPirate.h"

//���긱���������Ѱ·����,1  ������Ѱ·��1��
static char* __outsize_walkpath = "155:327|147:326|139:325|129:323";

//M��������λ��: 2  ������������-�ţ���3�롣��M���������������λ�ã���ѡ������M����1�롣��-�š�
static posxy_t __m_offset[] =
{
    //����1������2,...������N��Mƫ��
    {-218, 179},
    
};

//M��������λ��: 3  ��-�ţ���3�롣��M���������������λ�ã�1����
static posxy_t __m_offset3 = {-297, 154};

//Ѱ·����: 3  ��-�ţ���3�롣��M���������������λ�ã�1������M����1�롣��-�š�Ѱ·��2����
static char* __outsize_walkpath2 = "139:325|147:326|152:327";

///////////////////////////////////////////////////////////////////////////////////////////
//˵��:���������ȴ������ʱ�����ý���
//���÷���:
// 1.�������һ���ȴ������ʱ��ѭ��
// 2.ע���������б�����'\'��β,Ҳ����˵,'\'�����пո�,tab�����������ַ�
#define pirate_fight_atpoint_405_314() {\
    pirate_fight_time(0, 15),\
}

#define pirate_fight_atpoint_422_269() {\
    pirate_fight_time(0, 25),\
}

#define pirate_fight_atpoint_459_405() {\
    pirate_fight_time(0, 30),\
}

#define pirate_fight_atpoint_458_436() {\
    pirate_fight_time(0, 20),\
}

#define pirate_fight_atpoint_287_439() {\
    pirate_fight_time(0, 15),\
}

#define pirate_fight_atpoint_292_381() {\
    pirate_fight_time(0, 45),\
}

#define pirate_fight_atpoint_464_110() {\
    pirate_fight_time(0, 45),\
}

#define pirate_fight_atpoint_382_92() {\
    pirate_fight_time(5, 10),\
}

#define pirate_fight_atpoint_177_28() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_217_209() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_284_253() {\
    pirate_fight_time(0, 60),\
}

#define pirate_fight_atpoint_194_145() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_171_244() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_181_320() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_185_377() {\
    pirate_fight_time(5, 15),\
}

#define pirate_fight_atpoint_137_434() {\
    pirate_fight_time(0, 50),\
}

#define pirate_fight_atpoint_63_299() {\
    pirate_fight_time(5, 10),\
}

#define pirate_fight_atpoint_64_183() {\
    pirate_fight_time(0, 10),\
}

//�����ȴ������ʱ�����ý���
///////////////////////////////////////////////////////////////////////////////////////////

#define FONTPIRATE_DIR "font\\pirate\\"
#define BMPPIRATE_DIR "bmp\\pirate\\"


static HANDLE __lock;
static void _EnterCriticalSection(void)
{
    ::WaitForSingleObject(__lock, INFINITE);
    api_log_MsgDebug("_EnterCriticalSection::����:�ɹ�");
    return;
}

static void _LeaveCriticalSection(void)
{
    (void)::ReleaseMutex(__lock);
    api_log_MsgDebug("_LeaveCriticalSection::����:�ɹ�");
    return;
}

static int pirateCheckkeystate(Idmsoft* dmobj, void* ctxt)
{
    if (1 == dmobj->GetKeyState(65))
    {
        dmobj->KeyUp(65);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(68))
    {
        dmobj->KeyUp(68);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(83))
    {
        dmobj->KeyUp(83);
        LUAMAN_WAIT_MS(50);
    }
    if (1 == dmobj->GetKeyState(87))
    {
        dmobj->KeyUp(87);
        LUAMAN_WAIT_MS(50);
    }

    dmobj->KeyDownChar("a");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("a");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("s");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("s");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("w");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("w");
    LUAMAN_WAIT_MS(50);

    dmobj->KeyDownChar("d");
    LUAMAN_WAIT_MS(50);
    dmobj->KeyUpChar("d");
    LUAMAN_WAIT_MS(50);

    return ERRNO_SUCCESS;
}

static int pirateChecklife(Idmsoft* dmobj, void* ctxt, int recusive)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int blueThreshold = luaman_config->blueThreshold;
    int maxBlueFiled = luaman_config->maxBlueFiled;
    int minBlue = luaman_config->minBlue;
    int code = 0;
    int bCopyoutPre = 1;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strBBlood;
    CStringArray bBlood;
    char* cstr = NULL;
    int sz = 0;
    long bBloodAva = 0;
    long bBloodTot = 0;
    int strBBloodLen = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    return ERRNO_SUCCESS;
}


static int pirateWalk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int dir = luaman_config->direction;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    struct luamanwalkstate walkstate;
    CStringArray tgtPosXY;
    CStringArray tgtPosXYe;
    int tgtNum = 0;
    char* ptr = NULL;
    int index = 0;
    long tposX = 0, tposY = 0;
    int xyc = 0;
    int rdcltErrorCounter = 0;
    CString strPosXY;
    time_t startWalkTime = time(NULL);
    time_t curTime = 0;
    double walkTimeDelta = 0.0;
    int posX = 0;
    int posY = 0;
    CStringArray posXY;
    int keyPauseTime = 20;
    VARIANT pposX, pposY;
    //VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    //long lposX2 = 0, lposY2 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    walkstate.xNeg = "w";
    walkstate.xPos = "s";
    walkstate.yNeg = "a";
    walkstate.yPos = "d";

    if (cdir != dir)
    {
        dir = cdir;
    }
    
    if (DIR_LEFT == dir)
    {
        walkstate.xNeg = "d";
        walkstate.xPos = "a";
        walkstate.yNeg = "w";
        walkstate.yPos = "s";
    }

    if (DIR_U == dir)
    {
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(200);
        dmobj->MiddleClick();
        LUAMAN_WAIT_MS(200);
    }

    tgtNum = SplitString(tgtPosXYs, '|', tgtPosXY);
    if (tgtNum < 0)
    {
        ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
        api_log_MsgError("·��:����:�Ƿ�:Ѱ·:ʧ��,tgtNum:%d, tgtPosXYs:%s", tgtNum, ptr);
        
        tgtPosXYs.ReleaseBuffer();
        return SNAIL_ERRNO_INVALID;
    }
    
    //ǰ����ÿ�������
    for (index = 0; index < tgtNum; ++index)
    {
        xyc = SplitString(tgtPosXY.GetAt(index), ':', tgtPosXYe);
        if (xyc < 2)
        {
            ptr = tgtPosXYs.GetBuffer(BUFFER_MAX);
            
            api_log_MsgError("·��:����:����:�Ƿ�:Ѱ·:ʧ��,index:%d, tgtPosXYs:%s, xyc:%d",
                index, ptr, xyc);
        
            tgtPosXYs.ReleaseBuffer();
            return ERRNO_INVALID;
        }

        tposX = atol(tgtPosXYe.GetAt(0));
        tposY = atol(tgtPosXYe.GetAt(1));

        api_log_MsgDebug("����:����:�ɹ�,index:%d, tposX:%ld, tposY:%ld", index, tposX, tposY);

        walkstate.yNegKeyDown = 0;
        walkstate.yPosKeyDown = 0;
        walkstate.xPosKeyDown = 0;
        walkstate.xNegKeyDown = 0;

        //Check_Terminate_Thread(luaman_task, dmobj);
        
        //ǰ����һĿ�������
        for (;;)
        {
        WALK_LABEL_NEXT:
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            //////////////////////////////////////////////////////////////////////////////
            //@bug:���Ѱ·ʱ���Ƿ񳬹���ֵ120��,�����������Ϊ�Ǳ���ס��,�˳� 2019/4/20 10:09:39
            //bug:�޸�Ϊ40�� 2022/01/10 21:57
            curTime = time(NULL);
            walkTimeDelta = difftime(curTime, startWalkTime);
            if (walkTimeDelta > 300.0)
            {
                (void)pirateCheckkeystate(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = pirateChecklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONTPIRATE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)pirateCheckkeystate(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;

                api_log_MsgWarn("��ͼ:����:��ȡ:ʧ��");
                goto WALK_LABEL_NEXT;
            }

            //������ǰ����
            xyc = SplitString(strPosXY, ':', posXY);
            if (xyc < 2)
            {
                ptr = strPosXY.GetBuffer(BUFFER_MAX);
                
                api_log_MsgWarn("��ͼ:����:����:ʧ��,strPosXY:%s, xyc:%d",
                    ptr, xyc);
            
                strPosXY.ReleaseBuffer();

                (void)pirateCheckkeystate(dmobj, ctxt);
                
                walkstate.yNegKeyDown = 0;
                walkstate.yPosKeyDown = 0;
                walkstate.xPosKeyDown = 0;
                walkstate.xNegKeyDown = 0;
                
                goto WALK_LABEL_NEXT;
            }

            posX = atol(posXY.GetAt(0));
            posY = atol(posXY.GetAt(1));

            if (posX - tposX > 1)
            {
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;

                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }
                    
                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    /* Y�����겻��,X���ƶ� */
                    dmobj->KeyDownChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 1;
                }
            }
            else if (posX - tposX < -1)
            {
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                    
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    dmobj->KeyDownChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 1;
                }
            }
            else
            {
                if (1 == walkstate.xPosKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xPosKeyDown = 0;
                }

                if (1 == walkstate.xNegKeyDown)
                {
                    dmobj->KeyUpChar(walkstate.xNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.xNegKeyDown = 0;
                }

                /* X�����겻��,ֻ�ж�Y�� */
                if (posY - tposY > 1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.yNeg);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yNegKeyDown = 1;
                }
                else if (posY - tposY < -1)
                {
                    /* ������ */
                    dmobj->KeyDownChar(walkstate.yPos);
                    LUAMAN_WAIT_MS(keyPauseTime);
                    walkstate.yPosKeyDown = 1;
                }
                else
                {
                    if (1 == walkstate.yNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yNegKeyDown = 0;
                    }

                    if (1 == walkstate.yPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.yPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.yPosKeyDown = 0;
                    }

                    if (1 == walkstate.xPosKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xPos);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xPosKeyDown = 0;
                    }

                    if (1 == walkstate.xNegKeyDown)
                    {
                        dmobj->KeyUpChar(walkstate.xNeg);
                        LUAMAN_WAIT_MS(keyPauseTime);
                        walkstate.xNegKeyDown = 0;
                    }

                    LUAMAN_WAIT_MS(keyPauseTime);

                    //��ǰĿ������㴦�����
                    break;
                }

                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            //WALK_LABEL_NEXT
        }//end:ǰ����һĿ�������

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
    }

    //Ѱ·�������,˳������Ŀ���յ������
    return SNAIL_ERRNO_SUCCESS;

LABEL_WALK_BLOCK_EXIT_CHECK:
    //����ͼ�Ƿ��˳�����������
    for (;;)
    {
        //�ȼ����Ѫ��
        code = pirateChecklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:Ѫ��:���:����:����");
            return code;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ѱ·:��ס:��ͼ:ƽ���ӹ�:�˳�,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        Delay(500);
        api_log_MsgDebug("Ѱ·:��ס:��ͼ:ƽ���ӹ�:�˳�,�����˳���");
    }

    return SNAIL_ERRNO_BLOCKED;
}

static int pirateFighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    api_log_MsgDebug("���:�ȴ�:%d��:��ʼ", waittime);
    
    if (waittime > 0)
    {
        Delay_With_Terminate_Check(waittime, luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
    }

    api_log_MsgDebug("���:�ȴ�:%d��:����", waittime);

    if (fighttime <= 0)
    {
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("���:%d��:����", fighttime);
        return SNAIL_ERRNO_SUCCESS;
    }
    
    api_log_MsgDebug("���:%d��:��ʼ", fighttime);
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > (fighttime * 1.0))
        {
            api_log_MsgDebug("���:%d��:���", fighttime);
            break;
        }

        api_log_MsgDebug("���:%d��:����", fighttime);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("���:%d��:����", fighttime);
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint1_01(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_405_314();

    api_log_MsgDebug("405:314:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("405:314:���:���");
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateFightatPoint1(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_422_269();

    api_log_MsgDebug("422:269:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("422:269:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int pirateFightatPoint2_01(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_459_405();

    api_log_MsgDebug("459:405:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("459:405:���:���");
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateFightatPoint2(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_458_436();

    api_log_MsgDebug("458:436:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("458:436:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint3(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_287_439();

    api_log_MsgDebug("287:439:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("287:439:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint4(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_292_381();

    api_log_MsgDebug("292:381:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("292:381:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint5(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_464_110();

    api_log_MsgDebug("464:110:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("464:110:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint6(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_382_92();

    api_log_MsgDebug("382:92:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("382:92:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint7(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_177_28();

    api_log_MsgDebug("177:28:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("177:28:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint8(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_217_209();

    api_log_MsgDebug("217:209:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("217:209:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint9(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_284_253();

    api_log_MsgDebug("284:253:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("284:253:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint10(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_194_145();

    api_log_MsgDebug("194:145:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("194:145:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint11(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_171_244();

    api_log_MsgDebug("171:244:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("171:244:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int pirateFightatPoint12_01(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_181_320();

    api_log_MsgDebug("181:320:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("181:320:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint12(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_185_377();

    api_log_MsgDebug("185:377:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("185:377:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint13(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_137_434();

    api_log_MsgDebug("137:434:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("137:434:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint14(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_63_299();

    api_log_MsgDebug("63:299:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("63:299:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint15(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    struct fighttimeinfo fti[] = pirate_fight_atpoint_64_183();

    api_log_MsgDebug("64:183:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)pirateFighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("64:183:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint16(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    int uflag = 0;
    int failrnr = 0;
    int i = 0;

    api_log_MsgDebug("������:���:��ʼ");
    
    for (;;)
    {
    Label_FightatPoint16_start:

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!uflag || 2 == uflag)
        {
            dmobj->MoveTo(5, 5);
            Delay(100);

            failrnr = 0;
            
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->KeyDownChar("u");
                Delay(20);
                dmobj->KeyUpChar("u");
                Delay(200);

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������޵�", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("�������޵�:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������޵�", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("�������޵�:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                failrnr++;
                if (2 == uflag && failrnr > 5)
                {
                    api_log_MsgDebug("������:���:��ʱ:�˳�");
                    return SNAIL_ERRNO_SUCCESS;
                }
                
                api_log_MsgDebug("�������޵�:����:ʧ��");
                Delay(200);
            }
        }

        api_log_MsgDebug("�������޵�:����:����");
        
        lastChckTime = time(NULL);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (idx = 0; idx < 20; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(15);
                dmobj->KeyUpChar("3");
                Delay(15);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 3.0)
            {
                api_log_MsgDebug("�������޵�:����:��ͣ");
                break;
            }
       }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //�����û�д���
       for (;;)
       {
			Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
			for(i = 0; i < 2; i++)
			{
				Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "�������޵�", "ffffff-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("������:��ɫ:����:����, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				}

				dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi3.txt");
				code = dmobj->FindStrFast(0, 27, 1279, 826, "�������޵�", "ff3300-101010", 0.9, &pposX, &pposY);
				dmobj->ClearDict(0);
				if (-1 != code)
				{
					lposX = pposX.lVal;
					lposY = pposY.lVal;
                
					api_log_MsgDebug("������:��ɫ:����:����, lposX:%ld, lposY:%ld", lposX, lposY);
					goto Label_FightatPoint16_start;
				 }
			}

            //��һ������ʱ��
            dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi1.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "������", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                uflag = 2;
                failrnr = 0;
                
                api_log_MsgDebug("������:����:������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("������:���:���");
            return SNAIL_ERRNO_SUCCESS;
       }
    }
    
    api_log_MsgDebug("������:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateFightatPoint17(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int idx = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    int uflag = 0;
    
    api_log_MsgDebug("����˹:���:��ʼ");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!uflag)
        {
            dmobj->MoveTo(5, 5);
            Delay(100);
        
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->KeyDownChar("u");
                Delay(20);
                dmobj->KeyUpChar("u");
                Delay(200);

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��ŭ����������˹", "ffffff-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("��ŭ����������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��ŭ����������˹", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("��ŭ����������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi4.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��ŭ����������˹", "ff3300-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    uflag = 1;
                    
                    api_log_MsgDebug("��ŭ����������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }
                
                api_log_MsgDebug("��ŭ����������˹:����:ʧ��");
                Delay(200);
            }
        }

        api_log_MsgDebug("��ŭ����������˹:����:����");
        
        lastChckTime = time(NULL);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (idx = 0; idx < 20; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(15);
                dmobj->KeyUpChar("3");
                Delay(15);
            }

            curTime = time(NULL);
            deltaTime = difftime(curTime, lastChckTime);
            if (deltaTime > 3.0)
            {
                api_log_MsgDebug("����˹:����:��ͣ");
                break;
            }
       }

        dmobj->MoveTo(5, 5);
        Delay(100);
        
       //�����û�д���
       for (;;)
       {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�뿪ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("�뿪ʱ��:����:�ɹ�:����:����, lposX:%ld, lposY:%ld", lposX, lposY);
                goto Label_end_fight17;
            }

            api_log_MsgDebug("�뿪ʱ��:����:ʧ��:����:����");
            break;
       }
    }

Label_end_fight17:
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }
    
    api_log_MsgDebug("��ŭ����������˹:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateSaveXstrong2Library(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    int findxstrong = 0;

    api_log_MsgDebug("cleanPackageX:0x%x", cleanPackageX);
    
    //���ұ�����ǩ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:�����������λ�� 2021/6/18 10:11:19
            //841:93->667:505 1037:687
            boxPosX0 = lposX2 - 174;
            boxPosY0 = lposY2 + 412;
            boxPosX1 = lposX2 + 196;
            boxPosY1 = lposY2 + 594;

            labelPosXY[0].x = lposX2 - 70;
            labelPosXY[0].y = lposY2 + 400;
            labelPosXY[1].x = lposX2 - 10;
            labelPosXY[1].y = lposY2 + 400;
            labelPosXY[2].x = lposX2 + 50;
            labelPosXY[2].y = lposY2 + 400;
            labelPosXY[3].x = lposX2 + 110;
            labelPosXY[3].y = lposY2 + 400;
            labelPosXY[4].x = lposX2 + 170;
            labelPosXY[4].y = lposY2 + 400;

            break;
        }

        api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("�ֿ�:����:�����, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    api_log_MsgDebug("�ֿ�:����:�����,׼������");

    //����ÿ�����������б����
    int pkgnum = 2;
    for (lndex = 0; lndex < pkgnum; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(200);
        dmobj->LeftDown();
        Delay(200);
        dmobj->LeftUp();
        Delay(200);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(200);

                api_log_MsgDebug("����:��������:����:%d:����:[%d,%d]:����:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                findxstrong = 0;

                //����'Сǿ|��ǿ|��ǿ'����
                dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi7.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    findxstrong = 1;

                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("�����:��������:�洢,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                    //break;
                }

                if (0 == findxstrong)
                {
                    dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi7.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, "�����׽�ָ", "a335ee-101010", 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        findxstrong = 2;

                        lposX1 = pposX1.lVal;
                        lposY1 = pposY1.lVal;

                        Delay(200);
                        dmobj->RightClick();
                        Delay(200);

                        api_log_MsgDebug("�����:�����׽�ָ:�洢,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
                        //break;
                    }
                }
            }
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}
static int pirateCleanPackage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    CString strArrangePackPosXYs;
    CStringArray arrangePackPosXYs;
    int tgtNum = 0;
    int tgtNum2 = 0;
    long posX = 0;
    long posY = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int index = 0;
    int jndex = 0;
    CString strPosXYtgt;
    CStringArray posXYtgt;
    CString strPosXY;
    CStringArray posXY;
    long posX1 = 0, posY1 = 0;
    long buildx = 0, buildy = 0;

    //��i��������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("������:����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("������:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("���:׼��:���");
    
    //916,126->786,522
    {
        buildx = lposX - 130;
        buildy = lposY + 396;
        
        dmobj->MoveTo(buildx, buildy);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        api_log_MsgDebug("������:���,��i�˳�, buildx:%ld, buildy:%ld", buildx, buildy);
    }
    
    //���°�i,����¼����I/II/III/IV/V������λ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //bug:�����������λ�� 2021/6/18 10:11:19
            //841:93->667:505 1037:687
            boxPosX0 = lposX2 - 174;
            boxPosY0 = lposY2 + 412;
            boxPosX1 = lposX2 + 196;
            boxPosY1 = lposY2 + 594;

            labelPosXY[0].x = lposX2 - 70;
            labelPosXY[0].y = lposY2 + 400;
            labelPosXY[1].x = lposX2 - 10;
            labelPosXY[1].y = lposY2 + 400;
            labelPosXY[2].x = lposX2 + 50;
            labelPosXY[2].y = lposY2 + 400;
            labelPosXY[3].x = lposX2 + 110;
            labelPosXY[3].y = lposY2 + 400;
            labelPosXY[4].x = lposX2 + 170;
            labelPosXY[4].y = lposY2 + 400;

            break;
        }

        api_log_MsgDebug("����:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("׼�����Ҳֿ�ͼ��");
    
    posX1 = 1069;
    posY1 = 48;

    //���ͼ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);

        //���֮�����"�ֿ�"����
        dmobj->SetDict(0, FONTPIRATE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "�ֿ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ֿ�:����:����,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ֿ�:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("׼������ֿⰴť, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
    
    //���'�ֿ�'����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);

        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("�ֿ�:����:����:����,�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }

        api_log_MsgDebug("�ֿ�:����:����:����,ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("׼��:����:����");

    //�������еĶ���
    pirateSaveXstrong2Library(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateBuyBlue(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX1 = 0, lposY1 = 0;
    long lposX2 = 0, lposY2 = 0;
    int posX1 = 0;
    int posY1 = 0;
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    long deltaX = 35;
    long  deltaY = 35;
    long  lineX = -1;
    long  lineY = -1;
    int index = 0;
    int jndex = 0;
    int lndex = 0;
    long posX = 0;
    long posY = 0;
    struct xyposition boxPosXYs[5 * 10];
    
    api_log_MsgDebug("����:��ʼ");

    posX1 = 1069;
    posY1 = 48;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(posX1, posY1);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        //���֮�����"�ֿ�"����
        dmobj->SetDict(0, FONTPIRATE_DIR"font-surebox.txt");
        code = dmobj->FindStrFast(590, 500, 687, 530, "�ֿ�", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("�̵�:�ֿ�:����:����:�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("�̵�:�ֿ�:����:����:ʧ��");
        Delay(500);
    }

    //����̵� 633:510->633:380
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX1, lposY1 - 130);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONTPIRATE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�̵�:����:����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�̵�:����:����:����:ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        //���֮�����"�̵�"����
        dmobj->SetDict(0, FONTPIRATE_DIR"font-shop.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�̵�", "ffffff-101010", 0.9, &pposX1, &pposY1);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX1 = pposX1.lVal;
            lposY1 = pposY1.lVal;

            api_log_MsgDebug("�̵�:�̵�:����:����:�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);
            break;
        }
        
        api_log_MsgDebug("�̵�:�̵�:����:����:ʧ��");
        Delay(500);
    }

    //498:204 -> 658:278
    for (;;)
    {
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�̵�:ȷ��:����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�̵�:ȷ��:����:����:ʧ��");
        Delay(500);
    }

    //��������ȷ��
    code = dmobj->SendString(hwnd, "200");
    if (1 != code)
    {
        api_log_MsgError("�̵�:���:200:����:ʧ��");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("����:���:200:����:�ɹ�");

    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�̵�:ȷ��:����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�̵�:ȷ��:����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("����:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int pirateCheckcopyfull(Idmsoft* dmobj, void* ctxt)
{
    int status = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int waitcopytime = luaman_config->Wait2Sleep;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��鸱�����Ƿ��д�boss,�����������"����޷��볡"
    dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "����޷��볡��", "ffffff-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������д�boss����ȥ, lposX:%ld, lposY:%ld", lposX, lposY);

        //����"�������д�boss����ȥ:ȷ��"����
        for (;;)
        {
            dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;

                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:�ɹ�, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("�������д�boss����ȥ:ȷ��:����:ʧ��");
        }

        //���"�������д�boss����ȥ:ȷ��"����
        for (;;)
        {
            dmobj->MoveTo(lposX3, lposY3);
            LUAMAN_WAIT_MS(300);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(300);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONTPIRATE_DIR"font-reenter.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�");
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            LUAMAN_WAIT_MS(500);
            api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:ʧ��");
        }

        api_log_MsgDebug("�������д�boss����ȥ:ȷ��:���:�ɹ�:���¶Ի�");
        return SNAIL_ERRNO_AGAIN;
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //û�н��븱��,Ҳû�г���"�������д�boss����ȥ",�����Ƿ���"�������³���"
    dmobj->SetDict(0, FONTPIRATE_DIR"font-copyfull.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 != code)
    {
        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:�������³�����,lposX:%ld, lposY:%ld",
            lposX, lposY);

        for (;;)
        {
            dmobj->MoveTo(lposX + 55, lposY + 100);
            LUAMAN_WAIT_MS(500);
            dmobj->LeftClick();
            LUAMAN_WAIT_MS(500);
            dmobj->MoveTo(5, 5);
            LUAMAN_WAIT_MS(300);

            dmobj->SetDict(0, FONTPIRATE_DIR"font-copyfull.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�������³�����", "fff6f5-202020", 0.9, &pposX3, &pposY3);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                lposX3 = pposX3.lVal;
                lposY3 = pposY3.lVal;
                
                api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�,lposX3:%ld, lposY3:%ld",
                    lposX3, lposY3);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("�������³�����:ȷ��:���:ʧ��");
            LUAMAN_WAIT_MS(500);
        }

        api_log_MsgDebug("�������³�����:ȷ��:���:�ɹ�:���¶Ի�");
        return SNAIL_ERRNO_AGAIN;;
    }
    
    return SNAIL_ERRNO_SUCCESS;
}
static int pirateTask(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int idx = 0;
    int retry_npc_num = 0;
    int alarmHostIndex = luaman_config->alarmHostIndex;

LABEL_start_this_copy:
    pathofposxys = __outsize_walkpath;
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", __outsize_walkpath);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", __outsize_walkpath);

    _EnterCriticalSection();//����

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    if (alarmHostIndex < 0 || alarmHostIndex >= LUAMAN_DIM(__m_offset))
    {
        _LeaveCriticalSection();
        
        for (;;)
        {
            api_log_MsgWarn("�������:M���:ƫ��λ��:��ȡ:�쳣", alarmHostIndex);
            Delay(1000);
        }
    }

    api_log_MsgDebug("�������:M���:ƫ��λ��:����:%d, x:%lld, y:%lld",
        alarmHostIndex,
        __m_offset[alarmHostIndex].x,
        __m_offset[alarmHostIndex].y);
    
    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + __m_offset[alarmHostIndex].x, lposY + __m_offset[alarmHostIndex].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-fartm.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����Զ����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����Զ����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����Զ����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("enter");
    Delay(500);
    dmobj->KeyUpChar("enter");
    Delay(500);
    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + __m_offset3.x, lposY + __m_offset3.y);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
    
    pathofposxys = __outsize_walkpath2;
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        _LeaveCriticalSection();
        
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", __outsize_walkpath2);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", __outsize_walkpath2);
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����1
    /////////////////////////////////////////////////////////////////////////////////
LABEL_retalk_with_NPC:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        (void)check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    //�Ի�:    638:108 -> 638:494
    //delta: 0:386
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY + 386);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = pirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:����:������:���:����");
            goto LABEL_retalk_with_NPC;
        }

        code = common_check_copy_access(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            _LeaveCriticalSection();
            
            api_log_MsgDebug("��Ա����:���:�ɹ�:����");
            goto LABEL_start_this_copy;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("ȷ��:����:ʧ��");
        Delay(500);
    }

    //���ȷ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = pirateCheckcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:���:������:���:����");
            goto LABEL_retalk_with_NPC;
        }

        code = common_check_copy_access(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            _LeaveCriticalSection();
            
            api_log_MsgDebug("��Ա����:���:�ɹ�:����");
            goto LABEL_start_this_copy;
        }
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 ==code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    _LeaveCriticalSection();//����
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    double cleanPackageTimeDelta = 0.0;
    time_t curTime = time(NULL);
    int cleanPackageTime =2;// luaman_config->cleanPackageTime;
    time_t cleanPackageTimeLast = time(NULL);

    //���������� ����ʱ�����1:����ʱ���, 0:����ʱ�����
    //�ް�ƽ 2023/1/22 22:03:11
    int cleanPackageWhenStart = 1; //luaman_config->cleanPackageWhenStart;

    if (1 == cleanPackageWhenStart)
    {
        cleanPackageTimeLast = 1;
    }
    
    api_log_MsgDebug("����:��ʼ");

    //��������:������������߼�
    if (0 == cleanPackageTimeLast)
    {
        pirateCleanPackage(dmobj, ctxt);
        cleanPackageTimeLast = time(NULL);
        pirateBuyBlue(dmobj, ctxt);
    }
    else
    {
        curTime = time(NULL);
        cleanPackageTimeDelta = difftime(curTime, cleanPackageTimeLast);
        if (cleanPackageTimeDelta >= (cleanPackageTime * 3600 * 1.0))
        {
            pirateCleanPackage(dmobj, ctxt);
            cleanPackageTimeLast = time(NULL);
            pirateBuyBlue(dmobj, ctxt);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    // ����2
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "381:407|381:394";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[381:394]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[381:394]:�ɹ�");
    
    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        dmobj->SetDict(0, FONTPIRATE_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // ����3
    /////////////////////////////////////////////////////////////////////////////////
	pathofposxys = "382:383|385:373|390:359|394:347|399:336|401:325|405:314";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[405:314]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[405:314]:�ɹ�");
    
    code = pirateFightatPoint1_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[405:314]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[405:314]:���:�ɹ�");

    pathofposxys = "409:303|413:292|416:280|422:269";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[422:269]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[422:269]:�ɹ�");
    
    code = pirateFightatPoint1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[422:269]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[422:269]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����4
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 590:642
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 268, lposY + 484);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

	pathofposxys = "461:359|461:371|460:381|459:393|459:405";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[459:405]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[459:405]:�ɹ�");

    code = pirateFightatPoint2_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[458:436]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[459:405]:���:�ɹ�");

    pathofposxys = "459:415|459:424|458:436";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[458:436]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[458:436]:�ɹ�");

    code = pirateFightatPoint2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[458:436]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[458:436]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����5
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 707:487
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 151, lposY + 329);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "310:471|303:464|294:452|287:439";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[287:439]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[287:439]:�ɹ�");

    code = pirateFightatPoint3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[287:439]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[287:439]:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����6
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "280:427|275:417|284:411|287:400|290:390|292:381";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[292:381]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[292:381]:�ɹ�");

    code = pirateFightatPoint4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[292:381]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[292:381]:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����7
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 354:567
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 504, lposY + 409);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "399:116";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[399:116]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[399:116]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�鿴", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�鿴:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�鿴:����:ʧ��");
        Delay(500);
    }

    //����鿴
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ����ٻ�", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("�ƶ����ٻ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ƶ����ٻ�:����:ʧ��");
        Delay(500);
    }

    //����ƶ����ٻ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ����ٻ�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ����ٻ�:���:�ɹ�");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�ƶ����ٻ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = "458:114|464:110";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[464:110]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[464:110]:�ɹ�");
    
    code = pirateFightatPoint5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[464:110]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[464:110]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����8
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 354:567
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 504, lposY + 409);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "391:117|390:107|387:99|382:92";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[382:92]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[382:92]:�ɹ�");

    code = pirateFightatPoint6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[382:92]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[382:92]:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����9
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "376:86|370:88|365:90|359:92|353:95|344:95|339:95|336:95";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[336:95]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[336:95]:�ɹ�");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�鿴", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�鿴:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�鿴:����:ʧ��");
        Delay(500);
    }

    //����鿴
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�鿴", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�鿴:���:�ɹ�");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("�鿴:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����10
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 266:355
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 592, lposY + 197);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[177:28]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[177:28]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����11
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 448:400
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 410, lposY + 242);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[217:209]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[217:209]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����12
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 492:463
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 366, lposY + 305);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    code = pirateFightatPoint9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[284:253]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[284:253]:���:�ɹ�");

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    
    /////////////////////////////////////////////////////////////////////////////////
    // ����13
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 353:383
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 526, lposY + 335);// �ް�ƽ �޸�:�����λ��
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(1, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    dmobj->KeyPressChar("u");
    Delay(500);
    dmobj->KeyUpChar("u");
    Delay(500);

    dmobj->KeyPressChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////


    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 353:383
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 505, lposY + 225);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "207:110|200:119|199:129|198:136|194:145";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[194:145]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[194:145]:�ɹ�");

    code = pirateFightatPoint10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[194:145]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[194:145]:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����14
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "188:153|183:159|178:169|172:180|169:191|167:204|167:213|167:222|169:233|171:244";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[171:244]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[171:244]:�ɹ�");

    code = pirateFightatPoint11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[171:244]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[171:244]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����15
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "174:255|176:266|177:280|179:290|181:305|181:320";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[181:320]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[181:320]:�ɹ�");

    code = pirateFightatPoint12_01(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[181:320]:���:ʧ��:code:%d", code);
        return code;
    }

	pathofposxys = "179:335|180:348|181:357|184:364|185:377";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[185:377]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[185:377]:�ɹ�");

    code = pirateFightatPoint12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[185:377]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[185:377]:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����16
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "180:395|175:402|168:409|162:416|153:423|145:429|137:434";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[137:434]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[137:434]:�ɹ�");

    code = pirateFightatPoint13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[137:434]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[137:434]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����17
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "126:436|119:434";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[119:434]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[119:434]:�ɹ�");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�Ի�:����:��ɫ:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        
        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }
    
    //����Ի�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ҫ�ƶ�", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("��Ҫ�ƶ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("��Ҫ�ƶ�:����:ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ҫ�ƶ�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��Ҫ�ƶ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("��Ҫ�ƶ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // ����18
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ݸ�", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���ݸ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ݸ�:����:ʧ��");
        Delay(500);
    }

    //866:31 -> 869:85
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + 3, lposY + 54);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    }

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    pathofposxys = "61:308|63:304|63:299";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[63:299]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[63:299]:�ɹ�");
    
    code = pirateFightatPoint14(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[63:299]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[63:299]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����19
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 476:233
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 382, lposY + 75);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    pathofposxys = "59:230|63:227|64:224|64:219|69:219|72:214|70:207:72:199|73:190|69:186|64:183";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[64:183]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[64:183]:�ɹ�");
    
    code = pirateFightatPoint15(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[64:183]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[64:183]:���:�ɹ�");
    
    /////////////////////////////////////////////////////////////////////////////////
    // ����20
    /////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 311:239
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 547, lposY + 81);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    code = pirateFightatPoint16(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("���޵�:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("���޵�:���:�ɹ�");

    /////////////////////////////////////////////////////////////////////////////////
    // ����21
    /////////////////////////////////////////////////////////////////////////////////
    api_log_MsgDebug("���޵�:���ո�:30��:��ʼ");
    
    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("���޵�:���ո�:30��:����");

    int retry_Label_leave_time = 0;
    
    //�Ҹ����뿪ʱ���������ʱ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�뿪ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�뿪ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_leave_time;
        }

        dmobj->SetDict(0, FONTPIRATE_DIR"font-TiShi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            goto Label_die_time;
        }
        

        api_log_MsgDebug("�뿪ʱ��:����ʱ��:����:ʧ��");
        Delay(500);
    }

    //�������ߵ�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("��ѭ��:������:������");
        Delay(1000);
    }

Label_leave_time:
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 273:242
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 585, lposY + 84);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    retry_npc_num = 0;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        if (retry_Label_leave_time > 1)
        {
            api_log_MsgDebug("�Ի�:����:ʧ��:��ʱ:��ת:�ȴ�:����:����:%d", retry_Label_leave_time);
            goto Label_term_copy;
        }

        retry_npc_num++;
        if (retry_npc_num > 5)
        {
            api_log_MsgDebug("�Ի�:����:ʧ��:��ʱ:����:%d", retry_Label_leave_time);
            
            retry_Label_leave_time++;
            goto Label_leave_time;
        }
        
        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    //����Ի�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ҫ�ٻ�����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("��Ҫ�ٻ�����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("��Ҫ�ٻ�����:����:ʧ��");
        Delay(500);
    }

    //���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ҫ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("��Ҫ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��Ҫ������:����:ʧ��");
        Delay(500);
    }

	//���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ҫ������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��Ҫ������:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("��Ҫ������:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////
    // ����22
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:34";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[64:34]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[64:34]:�ɹ�");

    code = pirateFightatPoint17(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("[64:34]:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("[64:34]:���:�ɹ�");
    goto Label_continue_Next_Cpy;
    
Label_die_time:
    /////////////////////////////////////////////////////////////////////////////////
    // ����23
    /////////////////////////////////////////////////////////////////////////////////
    code = pirateFightatPoint17(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����ʱ��:���:ʧ��:code:%d", code);
        return code;
    }

    api_log_MsgDebug("����ʱ��:���:�ɹ�");

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    //��M��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }

    // 858:158 -> 273:242
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 585, lposY + 84);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);
    }

    //�ص�M
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:�ر�:ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
Label_continue_Next_Cpy:
    /////////////////////////////////////////////////////////////////////////////////
    // ����24
    /////////////////////////////////////////////////////////////////////////////////
    pathofposxys = "64:31";
    pathOfPosXYs = CString(pathofposxys);
    code = pirateWalk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[64:31]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[64:31]:�ɹ�");

    dmobj->KeyPressChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua9.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    //����鿴
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-DuiHua7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ֻ�����˸�����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("ֻ�����˸�����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("ֻ�����˸�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "лл", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("лл:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("лл:����:ʧ��");
        Delay(500);
    }

    //���лл
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "лл", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("лл:���:�ɹ�");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("лл:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

Label_term_copy:
    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //��������ͷ��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->SetDict(0, FONTPIRATE_DIR"font-MingZi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "����Ⱥ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("����Ⱥ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����Ⱥ��:����:ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("����:����");
    api_log_MsgDebug("����:����:�ȴ�:27��:��ʼ");
    
    //Delay_With_Terminate_Check(27, luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    api_log_MsgDebug("����:����:�ȴ�:27��:����");
    
    
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////
int api_TaskPirateInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    time_t start = time(NULL);
    
    api_log_MsgDebug("����:Pirate:��ʼ��: hwnd:%ld, role:%d", hwnd, (int)role);
    api_PrintConfig(luaman_config);

    /* �󶨴��� */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind Pirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind Pirate hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock Pirate hwnd failed, hwnd:%ld", hwnd);

        return ERRNO_BINDWND;
    }

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    api_log_MsgDebug("����:Pirate:��ʼ��:���, hwnd:%ld", hwnd);   
    return ERRNO_SUCCESS;
}

int api_TaskPirateMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    int j = 0;
    
    api_log_MsgDebug("����:Pirate:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    for (j = 0; j < luaman_config->loop_num; j++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        api_log_MsgDebug("ִ�и���,j:%ld, loop_num:%d", j, luaman_config->loop_num);
        
        code = pirateTask(dmobj, ctxt);
        if (code)
        {
            api_log_MsgDebug("ִ�и������,j:%ld, loop_num:%d", j, luaman_config->loop_num);
            break;
        }
    }

    api_log_MsgDebug("����:Pirate:��ѭ��:����, hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}

int api_TaskPirateExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;

    api_log_MsgDebug("����:Pirate:����: hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}

int api_TaskPiratePrepare(void)
{
    __lock = CreateMutex(NULL, FALSE, NULL);
    if (!__lock)
    {
        api_log_MsgError("api_TaskPiratePrepare failed");
        exit(1);
    }

    api_log_MsgDebug("api_TaskTeamgroupPrepare success");
    return 0;
}

