#include <math.h>
#include <stdlib.h>
#include "stdafx.h"
#include "xrrno.h"
#include "xlog.h"
#include "xcommon.h"
#include "xlist.h"
#include "task.h"
#include "config.h"
#include "alarm.h"
#include "task.h"
#include "taskSignleCpy1.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"

/*
source insight���ü���:
  1.�����ؼ���:
    (1).˫��ѡ��Ҫ�����Ĺؼ���
    (2).ͬʱ��סctrl��/�������������Ի���
    (3).���searchִ������,����ж�����ؽ��,�����淽������鿴
      <a>.������ļ�ͷ,�Զ�������Ӧ������
      <b>.ͬʱ��סalt�Ͷ��ż�,���ص��������,�����鿴��һ��,��������
*/
////////////////////////////////////////////////////////////////////////////////
//����7����������ƫ��ֵ
static struct luaman_posxy deltaatpos_single_cpy7[] = 
{
    {-321, 310},
    {-314, 325},
    {-413, 324},
    {-410, 245},
    {-315, 242},
    {-364, 204},
    {-362, 185},
    {-363, 151},
    {-348, 67},
};

#define signlecpy7_fight_atpoint_1() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_2() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_3() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_4() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_5() {\
    pirate_fight_time(5, 12),\
}

#define signlecpy7_fight_atpoint_6() {\
    pirate_fight_time(2, 22),\
}

#define signlecpy7_fight_atpoint_7() {\
    pirate_fight_time(10, 12),\
}

#define signlecpy7_fight_atpoint_8() {\
    pirate_fight_time(2, 25),\
}

#define signlecpy7_fight_atpoint_9() {\
    pirate_fight_time(1, 5),\
}

#define signlecpy7_fight_atpoint_10() {\
    pirate_fight_time(1, 5),\
}

#define signlecpy7_fight_atpoint_11() {\
    pirate_fight_time(1, 27),\
}


//�ҵ��˵����ɫ�����־�����(font-DuiHua4.txt)�����λ��
static struct luaman_posxy single_cpy7_ips_offset[] = 
{
    {31, 70},
    {31, 70},
    {31, 70},
    {31, 70},
};

//11����-�ţ���3�룬��M���������������λ�ã�7������-����1�롣�ٰ�M���һ�ɫ�Ľ��ʯ(font-DuiHua5.txt)��
//    �ҵ��˵����ɫ�Ľ��ʯ�����λ�á�
static struct luaman_posxy single_cpy7_jjs_offset[] = 
{
    {20, 76},
};
//����7���ý���
////////////////////////////////////////////////////////////////////////////////


//���˽��е�3����������������λ��
static struct luaman_posxy deltaatpos_single_cpy_foolish[] = 
{
    {-563, 80},
    {-511, 126},
    {-446, 158},
};

// 3  ������������58:70.��-�ż�����3�룬��M���������������λ�ã�1��.
//   ��0����1�롣��ʼ���1.�����Լ���ʱ�䣬�ȶ��٣�������Ǹ�����
// ʱ��: �ȶ��٣������
#define signlecpy_foolish_fight_atpoint_1() {\
    pirate_fight_time(30, 210),\
}

//5  ��-�ż�����3�룬��M���������������λ�ã�1������0����1�롣��ʼ��֣�
//   һ�ߴ�һ�������Ͻǵ�����ȴ�(font-foolish2.txt)��
//   �ҵ��˾�ֹͣ�����ո�30�¡�
//����ȴ�����ɫ
#define signlecpy_foolish_diewait_font_color "fdb602-101010"

////////////////////////////////////////////////////////////////////////////////
/* ����������ƫ��ֵ */
static struct luaman_posxy deltaatpos_single_cpy6[] = 
{
    {-250, 324},
    {-12, -13},//6��������ٰ�N����Ի������λ��2��deltaatpos_single_cpy6 ��2����
    {-366, 258},
    {-373, 218},
};

//����7���ȴ��̶�ʱ��(��λ:��)
static int single_cpy_wait_seconds = 30;

//8.��N�Ի������ħ����font-cpy6_npc6.txt����Ȼ���֣����ȼ������ķ�ʽ���򼸸��غϣ�
#define signlecpy1_fight_atpoint_1() {\
    pirate_fight_time(35, 35),\
    pirate_fight_time(35, 35),\
    pirate_fight_time(30, 30),\
}


////////////////////////////////////////////////////////////////////////////////////////////////
#define FONTSIGNLECPY1_DIR "font\\signle\\"
#define BMPSIGNLECPY1_DIR "bmp\\signle\\"

static HANDLE g_signlecpy_criticalSection;
void signlecpy_InitCriticalSection()
{
    g_signlecpy_criticalSection = ::CreateMutex(NULL, FALSE, NULL);
    if (NULL == g_signlecpy_criticalSection)
    {
        api_log_MsgError("create signlecpy_InitCriticalSection lock failed");
        exit(1);
    }
    return;
}

static void signlecpyEnterCriticalSection()
{
    ::WaitForSingleObject(g_signlecpy_criticalSection, INFINITE);
    return;
}

static void signlecpyLeaveCriticalSection()
{
    (void)::ReleaseMutex(g_signlecpy_criticalSection);
    return;
}

static void signlecpyDeleteCriticalSection()
{
    (void)::CloseHandle(g_signlecpy_criticalSection);
    return;
}

static int signle_checkkeystatecpy1(Idmsoft* dmobj, void* ctxt)
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

static int signle_checklifecpy1(Idmsoft* dmobj, void* ctxt, int recusive)
{
    return 0;
}

static int signle_walkcpy1(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
            if (walkTimeDelta > 30.0)
            {
                (void)signle_checkkeystatecpy1(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = signle_checklifecpy1(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)signle_checkkeystatecpy1(dmobj, ctxt);
                
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

                (void)signle_checkkeystatecpy1(dmobj, ctxt);
                
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
        code = signle_checklifecpy1(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:Ѫ��:���:����:����");
            return code;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
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

static int beat_cpy_timeout(Idmsoft* dmobj, void* ctxt, int tottime, int enterspace)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > (tottime * 1.0))
        {
            api_log_MsgDebug("����:%d��:���", tottime);
            break;
        }

        api_log_MsgDebug("����:%d��:����", tottime);
    }

    if (enterspace)
    {
        for (idx = 0; idx < 30; idx++)
        {
            dmobj->KeyDownChar("space");
            Delay(20);
            dmobj->KeyUpChar("space");
            Delay(20);
        }
    }
    
    api_log_MsgDebug("����:�׶�:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int beat_cpy1(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����1:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����1:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
    }

    lastChckTime = time(NULL);
    
    //��5��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 5.0)
        {
            api_log_MsgDebug("����1:��:5��:���");
            break;
        }

        api_log_MsgDebug("����1:��:5��:����");
    }

    api_log_MsgDebug("����1:���ո�:15��:��ʼ");
    
    for (idx = 0; idx < 250; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);

        if (idx % 20)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
        }
    }

    api_log_MsgDebug("����1:���ո�:15��:����");

    //TODO:�ȴ�һ���̶�ʱ����ȥ �ް�ƽ 2023/1/8 23:06:11
    Delay_With_Terminate_Check(25, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(50);
    dmobj->KeyUpChar("0");
    Delay(50);

    //����ͼ�ǲ��Ǳ�ɴ��;������㳡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���;������㳡", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���;������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���;������㳡:����:ʧ��");
        Delay(1000);
    }

    api_log_MsgDebug("����1:ִ��:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy2(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����2:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����2:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
    }

    lastChckTime = time(NULL);
    
    //��5��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(20);
        dmobj->KeyUpChar("u");
        Delay(20);

        for (idx = 0; idx < 40; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 5.0)
        {
            api_log_MsgDebug("����2:��:5��:���");
            break;
        }

        api_log_MsgDebug("����2:��:5��:����");
    }

    api_log_MsgDebug("����2:���ո�:15��:��ʼ");
    
    for (idx = 0; idx < 100; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����2:���ո�:15��:����");

    //TODO:�ȴ�һ���̶�ʱ����ȥ
    Delay_With_Terminate_Check(10, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(50);
    dmobj->KeyUpChar("0");
    Delay(50);

    //����ͼ�ǲ��Ǳ�ɴ��;������㳡
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���;������㳡", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���;������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���;������㳡:����:ʧ��");
        Delay(1000);
    }

    api_log_MsgDebug("����2:ִ��:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase1(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-9-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ͨ�����Ӽ��ҵ�·������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ͨ�����Ӽ��ҵ�·������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (50 * 1.0))
        {
            api_log_MsgDebug("ͨ�����Ӽ��ҵ�·������:���:��ʱ");
            break;
        }
        
        api_log_MsgDebug("ͨ�����Ӽ��ҵ�·������:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����3:�׶�1:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase2(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-20-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ͨ�������㳡��·������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ͨ�������㳡��·������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("ͨ�������㳡��·������:���:��ʱ");
            break;
        }

        api_log_MsgDebug("ͨ�������㳡��·������:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����3:�׶�2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase3_0(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    int istimeout = 0;

    dmobj->MoveTo(5, 5);
    Delay(200);

    api_log_MsgDebug("����3:�׶�3_0:���:��ʼ");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:����:ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:���:��ʼ");
    
    lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        if (0 == istimeout)
        {
            for (idx = 0; idx < 40; idx++)
            {
                dmobj->KeyDownChar("3");
                Delay(20);
                dmobj->KeyUpChar("3");
                Delay(20);
            }

            //�����ֳ���2����,�Ͱ�һ��~,Ȼ��һֱ�����Ƿ���� 2023/3/28 22:40:11
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (70 * 1.0))
            {
                api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:���:��ʱ");
                
                istimeout = 1;
                
                dmobj->KeyDownChar("~");
                Delay(200);
                dmobj->KeyUpChar("~");
                Delay(200);
            }
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:�ɹ�");
          
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:�ɹ�");

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi6.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��ϴ�ԵĻ�֮�ػ���", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:�ɹ�:����");
                    break;
                }
                else
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;
                    
                    api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:����, lposX:%ld, lposY:%ld", lposX, lposY);
                }
            }
            else
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:����, lposX:%ld, lposY:%ld", lposX, lposY);
            }
        }
        else
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("��ϴ�ԵĻ�֮�ػ���:��ɫ:���:����, lposX:%ld, lposY:%ld", lposX, lposY);
        }
        
        Delay(500);
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����3:�׶�3_0:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int beat_cpy3_phase3(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-32-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ͨ������Ļ�������Ⱦ��ͥԺ��·������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ͨ������Ļ�������Ⱦ��ͥԺ��·������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (200 * 1.0))
        {
            api_log_MsgDebug("ͨ������Ļ�������Ⱦ��ͥԺ��·������:���:��ʱ");
            break;
        }
        
        api_log_MsgDebug("ͨ������Ļ�������Ⱦ��ͥԺ��·������:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 50; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����3:�׶�3:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase4(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ⱦ��ͥԺ�Ľ��ʯ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��Ⱦ��ͥԺ�Ľ��ʯ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (180 * 1.0))
        {
            api_log_MsgDebug("��Ⱦ��ͥԺ�Ľ��ʯ������:���:��ʱ");
            break;
        }
        
        api_log_MsgDebug("��Ⱦ��ͥԺ�Ľ��ʯ������:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����3:�׶�4:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase5(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 40.0)
        {
            api_log_MsgDebug("����3:�׶�5:40��:���");
            break;
        }

        api_log_MsgDebug("����3:�׶�5:40��:����");
    }

    for (idx = 0; idx < 20; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(50);
        dmobj->KeyUpChar("space");
        Delay(50);
    }

    api_log_MsgDebug("����3:�׶�5:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy3_phase6(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����3:�׶�6:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����3:�׶�6:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����3:�׶�6:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy4_phase2(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 30; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(30);
            dmobj->KeyUpChar("3");
            Delay(30);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa127.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����1:�׶�6:�����ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����1:�׶�6:�����ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    code = beat_cpy_timeout(dmobj, ctxt, 10, 0);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    for (idx = 0; idx < 120; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����3:�׶�6:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy5_phase1(Idmsoft* dmobj, void* ctxt)
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
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����5:�׶�1:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����5:�׶�1:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����5:�׶�1:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy5_phase2(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ԩ�ĺ�������", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��Ԩ�ĺ�������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ԩ�ĺ�������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��Ԩ�ĺ�������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZi5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ԩ�ĺ�������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��Ԩ�ĺ�������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("��Ԩ�ĺ�������:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-waittime.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ȴ�ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����5:�׶�2:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����5:�׶�2:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����5:�׶�2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int cpy1_join2team(Idmsoft* dmobj, void* ctxt)
{
    int code = 0;
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    int syncCountMax = 2/*luaman_config->syncCountMax*/;
    struct xyposition syncPosXYs[4] =
    {
        {1234, 266},
        {1272, 286},
        {1234, 298},
        {1272, 318},
    };
    time_t lastChckTime = time(NULL);
    time_t curTime = time(NULL);
    double deltaTime = 0.0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    struct xyposition fellowPosXYs[2];
    time_t lastChckTimeAll;
    int i = 0, j = 0;
    long posX = 0, posY = 0;
    long posX1 = 0, posY1 = 0;
    long posX2 = 0, posY2 = 0;
    int syncCount = 0;
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("f");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f");
        LUAMAN_WAIT_MS(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ӻ���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���:����:��Ӻ���:����:��Χ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("���:����:��Ӻ���:����:��Χ,ʧ��:��ʱ");
            return SNAIL_ERRNO_FAILED;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("���:����:��Ӻ���:����:��Χ,ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    LUAMAN_WAIT_MS(200);

    lastChckTime = time(NULL);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
        code = dmobj->FindStrFast(350, 35, 1279, 826, "Ƶ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���:����:Ƶ��:����,�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTime);
        if (deltaTime > 20.0)
        {
            api_log_MsgDebug("���:����:Ƶ��:����,ʧ��,��ʱ,�˳�");
            
            return SNAIL_ERRNO_OFFLINE;
        }

        api_log_MsgDebug("���:����:Ƶ��:����,ʧ��");
        Delay(500);
    }

    fellowPosXYs[0].x = lposX2;
    fellowPosXYs[0].y = lposY2;
    fellowPosXYs[1].x = lposX2 - 120;
    fellowPosXYs[1].y = lposY2 + 20;

    lastChckTimeAll = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX = fellowPosXYs[i].x;
            posY = fellowPosXYs[i].y;
            
            api_log_MsgDebug("i:%d, posX:%ld, posY:%ld", i, posX, posY);

            lastChckTime = time(NULL);

            //�����Ա����,��������Ի���
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(posX, posY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "7d7d7d-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("���:����:��Ա:����:����,�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                curTime = time(NULL);
                deltaTime = difftime(curTime, lastChckTime);
                if (deltaTime > 20.0)
                {
                    api_log_MsgDebug("���:����:��Ա:����:����,ʧ��,��ʱ,�˳�");
                    return SNAIL_ERRNO_OFFLINE;
                }

                api_log_MsgDebug("���:����:��Ա:����:����,ʧ��");
                Delay(500);
            }

            //���'����'������Ѽ������
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
        }

        syncCount = 0;

        //@bug:�Ե�Ƭ��,�����������˷�ʱ��2019/4/8 23:06:19
        Delay(2000);

        api_log_MsgDebug("ͬ��:����:���");
        
        //���'ͬ��'����
        for (i = 0; i < 2; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            posX1 = syncPosXYs[2 * i].x;
            posY1 = syncPosXYs[2 * i].y;
            posX2 = syncPosXYs[2 * i + 1].x;
            posY2 = syncPosXYs[2 * i + 1].y;

            api_log_MsgDebug("(%ld, %ld) --> (%ld, %ld)", posX1, posY1, posX2, posY2);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-inviteheil2team-1.txt");
            code = dmobj->FindStrFast(posX1, posY1, posX2, posY2, "ͬ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                syncCount++;

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("ͬ��:���:�ɹ�:%ld:%ld", lposX2, lposY2);
            }
        }

        if (syncCount == syncCountMax)
        {
            api_log_MsgDebug("���:����:��Ա:����,������");
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, lastChckTimeAll);
        if (deltaTime > 30.0)
        {
            api_log_MsgDebug("���:����:��Ա:����:����,ʧ��,��ʱ,�˳�");
            
            if (0 == syncCount)
            {
                return SNAIL_ERRNO_FAILED;
            }

            return SNAIL_ERRNO_AGAIN;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("���:����:��Ա:����,���δ���,syncCount:%d", syncCount);
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    api_log_MsgDebug("������:�ɹ�:���");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy1(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    long copylefttimem = 0;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    int rdcltErrorCounter = 0;
    char* ptr = NULL;
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);
    

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����(��", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����(��3��)", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            dmobj->KeyPressChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);
            
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��3��):����:��Χ:�ɹ�:�˳�:����, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��3��):����:ʧ��");
        Delay(500);

        break;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����(��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        
        Delay(500);
    }

    api_log_MsgDebug("�ٽ���:����:�ȴ�");
    signlecpyEnterCriticalSection();
    api_log_MsgDebug("�ٽ���:����:�ɹ�");
    
    code = cpy1_join2team(dmobj, ctxt);

    api_log_MsgDebug("�ٽ���:�˳�:�ȴ�");
    signlecpyLeaveCriticalSection();
    api_log_MsgDebug("�ٽ���:�˳�:�ɹ�");
    
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("����1:���:ʧ��:%d", code);
        return code;
    }
    
LABEL_retalk_with_NPC_in_cpy1:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:ʧ��");
        Delay(500);
    }

    //��ڶ��� 53:497->53:461
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 36);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy1;
        }
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy1;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("ȷ��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���ͽǶ���������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���ͽǶ���������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���ͽǶ���������:����:ʧ��");
        Delay(1000);
    }

    pathofposxys = "265:235|274:240|278:247|278:255|272:256|265:256|256:256";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[256:256]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[256:256]:�ɹ�");

    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }

    /*
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copylefttime.txt");
        strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strCopylefttime != CString(""))
        {
            count = SplitStringWide(strCopylefttime, "��", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem == 11)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("����:����ʱ��:����:�ȴ�:����:��ʱ:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto beat_start;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("����:����ʱ��:%s", ptr);
                strCopylefttime.ReleaseBuffer();
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("����:����ʱ��:����:�ȴ�");
        LUAMAN_WAIT_MS(1000);
    }*/

beat_start:

    Delay_With_Terminate_Check(200, luaman_task, dmobj);

    //��ʼ���
    status = beat_cpy1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS == status)
    {
        api_log_MsgError("����1:���:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����1:���:�ɹ�:����:��һ��");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy2(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    long copylefttimem = 0;
    CString strCopylefttime;
    int count = 0;
    CStringArray copylefttime;
    int rdcltErrorCounter = 0;
    char* ptr = NULL;

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:ʧ��");
        Delay(500);
    }

    //53:497->53:479
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 18);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ����������볡ȯ����(��", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���ͽǶ����������볡ȯ����(��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("���ͽǶ����������볡ȯ����(��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ����������볡ȯ����(��3��)", "00ff00-101010", 0.9, &pposX3, &pposY3);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX3 = pposX3.lVal;
            lposY3 = pposY3.lVal;

            dmobj->KeyPressChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);
            
            api_log_MsgDebug("���ͽǶ����������볡ȯ����(��3��):����:��Χ:�ɹ�:�˳�:����, lposX3:%ld, lposY3:%ld", lposX3, lposY3);
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("���ͽǶ����������볡ȯ����(��3��):����:ʧ��");
        Delay(500);
        
        break;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ����������볡ȯ����(��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����(��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy2:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk1.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "���ͽǶ���������(���)�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ͽǶ���������(���)�볡ȯ����:����:ʧ��");
        Delay(500);
    }

    //���1�� 53:497->53:443
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 50);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc2:�Ի�:ȷ��:����:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy2;
        }
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc2:�Ի�:ȷ��:����:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy2;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("ȷ��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���ͽǶ���������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���ͽǶ���������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���ͽǶ���������:����:ʧ��");
        Delay(1000);
    }

    Delay_With_Terminate_Check(130, luaman_task, dmobj);

    pathofposxys = "248:234";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[248:234]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[248:234]:�ɹ�");
    
    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }
    
    /*for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copylefttime.txt");
        strCopylefttime = dmobj->Ocr(15, 172, 183, 194, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strCopylefttime != CString(""))
        {
            count = SplitStringWide(strCopylefttime, "��", copylefttime);
            if (count > 2)
            {
                copylefttimem = atol(copylefttime.GetAt(0));
                if (copylefttimem == 11)
                {
                    ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                    api_log_MsgDebug("����:����ʱ��:����:�ȴ�:����:��ʱ:%s", ptr);
                    strCopylefttime.ReleaseBuffer();
                    
                    goto beat_start;
                }

                ptr = strCopylefttime.GetBuffer(BUFFER_MAX);
                api_log_MsgDebug("����:����ʱ��:%s", ptr);
                strCopylefttime.ReleaseBuffer();
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        api_log_MsgDebug("����:����ʱ��:����:�ȴ�");
        LUAMAN_WAIT_MS(1000);
    }*/

beat_start:

    Delay_With_Terminate_Check(80, luaman_task, dmobj);

    //��ʼ���
    status = beat_cpy2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS == status)
    {
        api_log_MsgError("����2:���:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����2:���:�ɹ�:����:��һ��");
    return SNAIL_ERRNO_SUCCESS;
}

int check_copyfull(Idmsoft* dmobj, void* ctxt)
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
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
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
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
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

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-reenter.txt");
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
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyfull.txt");
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

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyfull.txt");
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

int signle_copy3(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    int retrytimes = 0;
    
    //��p���˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�˳�����:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[�����Ŀ������]�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[�����Ŀ������]�볡ȯ����:����:ʧ��");
        Delay(500);
    }

    //���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����������:���:�ɹ�");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("�����Ŀ�������볡ȯ����:����:ʧ��");
        Delay(500);
    }

    //���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            return SNAIL_ERRNO_NOMORE;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100001.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�����Ŀ�������볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("�����Ŀ�������볡ȯ����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC3:
    
    //�������Ի�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->KeyUpChar("n");
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-2-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����Ŀ������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�����Ŀ������:����:ʧ��");
        LUAMAN_WAIT_MS(500);
    }
    
    //���"�����Ŀ������"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-enter-ack.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�����Ŀ������:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("�����Ŀ������:���:������:���:����");
            goto LABEL_retalk_with_NPC3;
        }
        
        api_log_MsgDebug("�����Ŀ������:���:ʧ��");
        LUAMAN_WAIT_MS(500);
    }

    //���"ȷ��"���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "�����Ŀ������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)//��������
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:�����Ŀ������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        //�����¸�����
        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("�����Ŀ������:����:������:���:����");
            goto LABEL_retalk_with_NPC3;
        }
        
        api_log_MsgDebug("�����Ŀ������:ȷ��:���:�ɹ�:����:Ŭ��������");
        LUAMAN_WAIT_MS(500);
    }

    //�ȴ�,������"��������ʱ��"ʱ��ʼ����
    for (;;)
    {
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-5-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������ʱ��:����:�ɹ�,lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("��������ʱ��:����:ʧ��");
        LUAMAN_WAIT_MS(1000);
    }
    
    pathofposxys = "108:415|117:415|128:415|140:414|151:414|154:421|157:428|167:429|176:429";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[176:429]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[176:429]:�ɹ�");

    status = beat_cpy3_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�1:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "184:429|192:429|198:422";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[198:422]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[198:422]:�ɹ�");

Label_redialog_cpy3_phase1:
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������Ӽ���:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ������Ӽ���:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ������Ӽ���"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-15-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ������Ӽ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ������Ӽ���:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("�ƶ������Ӽ���:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    retrytimes = 0;
    
    //��鵱ǰ����,������겻��254:415������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 254) <= 1 && abs(posY - 415) <= 1)
                {
                    api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 197) <= 1 && abs(posY - 421) <= 1)
                {
                    
                }
            }
        }

        retrytimes++;
        if (retrytimes > 10)
        {
            beat_cpy_timeout(dmobj, ctxt, 10, 0);
            
            api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:����:��ȡ:ʧ��:%d:���¶Ի�", retrytimes);
            goto Label_redialog_cpy3_phase1;
        }
        
        api_log_MsgDebug("�ƶ������Ӽ���:�Ի�:����:��ȡ:ʧ��:%d", retrytimes);
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "263:415|274:417|273:419";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[273:419]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[273:419]:�ɹ�");

    Delay_With_Terminate_Check(50, luaman_task, dmobj);

    //���
    status = beat_cpy3_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�2:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //����16
    pathofposxys = "281:424|291:426|300:427|309:427|319:427|329:427|348:428|356:429";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[356:429]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[356:429]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ��������㳡:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ��������㳡:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ��������㳡"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(380, 27, 1279, 826, "�ƶ��������㳡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������㳡:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("�ƶ��������㳡:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 374) <= 1 && abs(posY - 428) <= 1)
                {
                    api_log_MsgDebug("�ƶ��������㳡:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 357) <= 1 && abs(posY - 429) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("�ƶ��������㳡:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    //pathofposxys = "383:422|387:412";
    pathofposxys = "383:422|383:412";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[383:412]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[383:412]:�ɹ�");

    //���
    status = beat_cpy3_phase3_0(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�3_0:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "383:412";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:��λ:����:[383:412]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:��λ:����:[383:412]:�ɹ�");
    
    //���
    status = beat_cpy3_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�3:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //17��
    pathofposxys = "382:397|380:390|375:387";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[375:387]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[375:387]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-34-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����һ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ�����һ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ�����һ����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-35-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:����:ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-27-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����Ⱦ��ͥԺ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 386) <= 1 && abs(posY - 312) <= 1)
                {
                    api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 375) <= 1 && abs(posY - 388) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("�ƶ�����Ⱦ��ͥԺ:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "385:303|383:291|382:280";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[382:280]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[382:280]:�ɹ�");

    Delay_With_Terminate_Check(50, luaman_task, dmobj);

    //���
    status = beat_cpy3_phase4(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�4:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //18��
    pathofposxys = "384:265|385:251|386:238|387:228|387:218|382:212";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[382:212]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[382:212]:�ɹ�");

    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ff0000-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("���ʯ:����:ʧ��");
        Delay(500);*/
    }

    //��`��
    dmobj->KeyDown(192);
    Delay(200);
    dmobj->KeyUp(192);
    Delay(200);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //����18:��M��
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    // 858:158 -> 631:560
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 227, lposY + 402);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("Ŀ��:����:%ld:%ld:����:�ɹ�",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("ǰ��:Ŀ��:����:%ld:%ld:��ǰ:����:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }
    

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    pathofposxys = "376:389";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[376:389]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[376:389]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-84-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����һ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ�����һ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ�����һ����:����:ʧ��");
        Delay(500);
    }

    //���"�ƶ�����һ����"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������Ļ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�ƶ�������Ļ���:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ƶ�������Ļ���:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-85-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�������Ļ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�������Ļ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�ƶ�������Ļ���:���:ʧ��");
        Delay(500);
    }

    //19��
    pathofposxys = "347:395|339:386|334:376|330:366|323:360|316:354|308:347|302:341|298:336|291:334";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[291:334]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[291:334]:�ɹ�");

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������Ļ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������Ļ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������Ļ���:����:ʧ��");
        Delay(500);
    }

    //���"��������Ļ���"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-94-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������Ļ���", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��������Ļ���:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("��������Ļ���:���:ʧ��");
        Delay(500);
    }

    //20��
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: ����20:��M��
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    // 858:158 -> 520:358
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 338, lposY + 200);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("Ŀ��:����:%ld:%ld:����:�ɹ�",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("ǰ��:Ŀ��:����:%ld:%ld:��ǰ:����:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    pathofposxys = "177:290";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[177:290]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[177:290]:�ɹ�");

    status = beat_cpy3_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�5:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }
    
    //22��
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: ����22:��M��
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    // 858:158 -> 595:338
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 263, lposY + 180);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("Ŀ��:����:%ld:%ld:����:�ɹ�",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("ǰ��:Ŀ��:����:%ld:%ld:��ǰ:����:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "161:355";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[161:355]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[161:355]:�ɹ�");

    status = beat_cpy3_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�5:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //23��
    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO: ����23:��M��
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    // 858:158 -> 528:338
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 330, lposY + 180);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("Ŀ��:����:%ld:%ld:����:�ɹ�",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("ǰ��:Ŀ��:����:%ld:%ld:��ǰ:����:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "158:281";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[158:281]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[158:281]:�ɹ�");
    
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-40-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ff0000-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("���ʯ:����:ʧ��");
        Delay(500);*/
    }

    //��`��
    dmobj->KeyDown(192);
    Delay(200);
    dmobj->KeyUp(192);
    Delay(200);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //TODO:����23:��m��
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    // 858:158 -> 344:425
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 514, lposY + 267);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pjhg_tgtpos[pjhg_tgtpos_index].posX) <= 3 && abs(posY - pjhg_tgtpos[pjhg_tgtpos_index].posY) <= 3)
                {
                    api_log_MsgDebug("Ŀ��:����:%ld:%ld:����:�ɹ�",
                        pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY);
                    break;
                }

                api_log_MsgDebug("ǰ��:Ŀ��:����:%ld:%ld:��ǰ:����:%ld:%ld",
                    pjhg_tgtpos[pjhg_tgtpos_index].posX, pjhg_tgtpos[pjhg_tgtpos_index].posY,
                    posX, posY);
            }
        }

        Delay(500);*/
    }

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "244:109";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[244:109]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[244:109]:�ɹ�");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-59-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��������:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("��������:���:ʧ��");
        Delay(500);
    }
    
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    //��鵱ǰ����,������겻�� 244:151 ������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 244) <= 1 && abs(posY - 151) <= 1)
                {
                    api_log_MsgDebug("��������:�Ի�:�ɹ�");
                    break;
                }
                else if (abs(posX - 244) <= 1 && abs(posY - 113) <= 1)
                {
                    
                }
            }
        }

        api_log_MsgDebug("��������:�Ի�:����:��ȡ:ʧ��");
        Delay(500);
    }
    
    //24��
    dmobj->KeyPressChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    for (code = 0; code < 5; code++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(200);
        dmobj->KeyUpChar("u");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        api_log_MsgDebug("����ŵ˹:u:����:%d", i);
        
        /*dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ŵ˹", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ŵ˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ŵ˹", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ŵ˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa-61-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ŵ˹", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����ŵ˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("����ŵ˹:����:ʧ��");
        Delay(500);*/
    }

    status = beat_cpy3_phase6(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����3:�׶�6:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }
    
    //25��
    pathofposxys = "244:159|244:166|244:172";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[244:172]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[244:172]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ƶ������������鷿:�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld",
                lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ������������鷿:�Ի�:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������������鷿", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("�ƶ��������������鷿:����:�ɹ�,lposX2:%ld, lposY2:%ld",
                lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ƶ��������������鷿:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-4-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ��������������鷿", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ��������������鷿:���:�ɹ�");
            break;
        }

        Delay(500);
        api_log_MsgDebug("�ƶ��������������鷿:���:ʧ��");
    }

    dmobj->MoveTo(5, 5);
    Delay(300);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc1000.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("���ص�����ŵ˹��������:����:�ɹ�, lposX:%ld, lposY:%d", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ص�����ŵ˹��������:����:ʧ��");
        Delay(500);
    }

    //������� 573:429 ->611:467~611:502~670:466~670:499
    //573+38/97, 429+38/70
    lposX = lposX + 38 + time(NULL) % 50;
    lposY = lposY + 38 + time(NULL) % 25;

    api_log_MsgDebug("����:���:���:λ��:lposX:%ld, lposY:%ld", lposX, lposY);
    
    dmobj->MoveTo(lposX, lposY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    //��20�¿ո�
    for (index = 0; index < 40; ++index)
    {
        dmobj->KeyDown(32);
        Delay(50);
        dmobj->KeyUp(32);
        Delay(50);
    }
    
    dmobj->KeyPressChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    //�Ի�,������
    dmobj->MoveTo(5, 5);
    Delay(300);

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->KeyPressChar("n");
        Delay(300);
        dmobj->KeyUpChar("n");
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�Ի�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("�Ի�:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ���뿪����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("ȷ���뿪����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        api_log_MsgDebug("ȷ���뿪����:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    for (;;)
    {
        dmobj->MoveTo(lposX2, lposY2);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(300);

        ///////////////////////////////////////////////////////////////////////////////////////    
        //bug:�����һ��֮��,�ֵ�����һ�� 2019/12/22 21:37:22
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aaa-12-1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ���뿪����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ���뿪����:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("ȷ���뿪����:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    api_log_MsgDebug("����3:���:�ɹ�:����:��һ��");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy4_phase1(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    api_log_MsgDebug("����4:�׶�1:���:��ʼ");
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa57.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����֮�������ˡ�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����֮�������ˡ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("����֮�������ˡ�:���:��ʱ");
            break;
        }

        api_log_MsgDebug("����֮�������ˡ�:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����4:�׶�1:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy4(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    int findOkFailedCount = 0;
    
    //��p���˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�˳�����:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //29��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[���׵Ļ�¯]�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[���׵Ļ�¯]�볡ȯ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[���׵Ļ�¯]�볡ȯ����:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׵Ļ�¯�볡ȯ����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���׵Ļ�¯�볡ȯ����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����������:���:�ɹ�");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }

        api_log_MsgDebug("���׵Ļ�¯�볡ȯ����:����:ʧ��");
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
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-join2team-2.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "���׵Ļ�¯�볡ȯ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("���׵Ļ�¯�볡ȯ����:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("���׵Ļ�¯�볡ȯ����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy4:
    //��n�Ի����븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[�볡]���׵Ļ�¯", "00ff00-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:[�볡]���׵Ļ�¯:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:[�볡]���׵Ļ�¯:����:��Χ,ʧ��");
        Delay(500);
    }

    //���'[�볡]���׵Ļ�¯'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);

        //����Ƿ����'ȷ��'�����ĶԻ���
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,ʧ��");
        Delay(500);
    }

    //���'ȷ��'�����Ի���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׵Ļ�¯", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯:����,�ɹ�:lposX:%ld,lposY:%ld", lposX, lposY);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯:ȷ��:����:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy4;
        }
        
        api_log_MsgDebug("npc1:�Ի�:���׵Ļ�¯,���ڽ���");
        Delay(500);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //������©�Ĳ��� 2023/1/15 11:06:11
    pathofposxys = "470:372|462:379|455:385";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[455:385]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[455:385]:�ɹ�");

    //@bug:��鸱������ʱ��,�оͲ����ٶԻ�4/8/2019 23:43:55
    dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-copyrunning.txt");
    code = dmobj->FindStrFast(0, 27, 1279, 826, "��������ʱ��:", "fdb602-101010", 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            //��n�Ի�
            dmobj->KeyPressChar("n");
            Delay(500);
            dmobj->KeyUpChar("n");
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(5, 5);
            Delay(200);

            //����'֪���ˡ�(��ʼ)'����
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(388, 115, 984, 667, "֪���ˡ�(��ʼ)", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }
            

            api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):����,ʧ��");
            Delay(500);
        }
        

        findOkFailedCount = 0;
        
        //���'֪���ˡ�(��ʼ)'����
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(5, 5);
            Delay(200);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc2.txt");
            code = dmobj->FindStrFast(388, 115, 984, 667, "֪���ˡ�(��ʼ)", "00ff00-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):���,�ɹ�");
                break;
            }

            findOkFailedCount++;
            if (findOkFailedCount > 10)
            {
                api_log_MsgDebug("npc2:�Ի�:֪���ˡ�(��ʼ):���:ʧ��:��ʱ,����:��������");
                goto LABEL_find_failed_after_npc_talk;
            }

            api_log_MsgDebug("npc2:�Ի�:�õ�:����,ʧ��");
            Delay(500);
        }
    }

LABEL_find_failed_after_npc_talk:
    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //30��
    pathofposxys = "449:383|440:383|431:383|420:383|410:382|399:382|393:382";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[393:382]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[393:382]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "381:384|372:384|362:384|353:384|346:384|340:383|333:383|325:383|318:383|309:382";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[309:382]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[309:382]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "306:372|304:362|304:355|303:349|302:343|299:338|294:333|292:326|291:318|292:307|293:298";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[293:298]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[293:298]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    //code = beat_cpy_timeout(dmobj, ctxt, 40, 1);
    code = beat_cpy4_phase1(dmobj, ctxt);//��Ϊһֱ��ֱ���ҵ���־ 2023/5/17 21:46:12
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "291:288|289:279|286:271|286:263|286:254";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[286:254]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[286:254]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 20, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "286:254|286:249|286:241|286:232|286:223|286:213|285:205|280:197|272:197|263:197";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[263:197]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[263:197]:�ɹ�");
    
    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }
    
    pathofposxys = "261:204|258:215|256:224|251:233|246:239|241:240|235:240|228:242|222:242|215:242|212:240|202:239";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[202:239]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[202:239]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "192:236|180:232|170:229|162:224|160:215|161:206|166:197|174:196";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[174:196]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[174:196]:�ɹ�");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);
    
    code = beat_cpy_timeout(dmobj, ctxt, 10, 1);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    pathofposxys = "186:193|194:189|199:183|199:175|199:167|199:159|199:152|199:145";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[199:145]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[199:145]:�ɹ�");

    //31��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa119.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������档", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�Ի�:�������档:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�Ի�:�������档:����:ʧ��");
        Delay(500);
    }

    //���'�������档'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa120.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ǣ���Ҫ��ȥ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:����:ʧ��");
        Delay(500);
    }

    //���'�ǣ���Ҫ��ȥ��'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa120.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ǣ���Ҫ��ȥ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�Ի�:�ǣ���Ҫ��ȥ��:���:ʧ��");
        Delay(500);
    }

    pathofposxys = "199:132|199:123|199:115";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[199:115]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[199:115]:�ɹ�");

    Delay_With_Terminate_Check(15, luaman_task, dmobj);
    
    code = beat_cpy4_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //31��
    pathofposxys = "200:99|199:92|200:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[200:89]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[200:89]:�ɹ�");

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa130.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�뿪������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�Ի�:�뿪������:����:�ɹ�:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("�Ի�:�뿪������:����:�ɹ�:ʧ��");
        Delay(500);
    }

    //���'�������档'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-aa130.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�뿪������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:�뿪������:���:�ɹ�");
            break;
        }

        api_log_MsgDebug("�Ի�:�뿪������:���:ʧ��");
        Delay(500);
    }

    api_log_MsgDebug("����4:�ɹ�:����:��һ��");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy5(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    
    //��p���˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�˳�����:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[�������:���޶�ս]�볡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[�������:���޶�ս]�볡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        
        api_log_MsgDebug("[�������:���޶�ս]�볡:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�������:���޶�ս�볡", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�������:���޶�ս�볡:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����������:���:�ɹ�");

                    

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("�������:���޶�ս�볡:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�������:���޶�ս�볡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�������:���޶�ս�볡:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�������:���޶�ս�볡:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

LABEL_retalk_with_NPC_in_cpy5:
    //���¶Ի�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "[�������:���޶�ս]�볡", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[�������:���޶�ս]�볡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[�������:���޶�ս]�볡:����:ʧ��");
        Delay(500);
    }

    //��� 50:444 -> 50:426
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY - 18);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("npc1:�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�������:���޶�ս�볡:����:ʧ��");
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

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:���:�������³�����:���¶Ի�");
            goto LABEL_retalk_with_NPC_in_cpy5;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("ȷ��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    Delay_With_Terminate_Check(10, luaman_task, dmobj);
    
    pathofposxys = "260:231|254:232|249:234";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[249:234]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[249:234]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����������:����:ʧ��");
        Delay(500);
    }

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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����������:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    start_tmstmp = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����������:����2:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        end_tmstmp = time(NULL);
        time_delta = difftime(end_tmstmp, start_tmstmp);
        if (time_delta >= (10 * 1.0))
        {
            api_log_MsgDebug("����������:����2:10��:��ʱ:����");
            goto Label_skip_opencpy2;
        }

        api_log_MsgDebug("����������:����2:ʧ��");
        Delay(500);
    }

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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����������:���2:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����������:���2:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

Label_skip_opencpy2:
    pathofposxys = "255:233|260:233|264:238|264:244";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[264:244]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[264:244]:�ɹ�");

    //5-1
    Delay_With_Terminate_Check(40, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����5:�׶�1:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 298) <= 1 && abs(posY - 277) <= 1)
                {
                    api_log_MsgDebug("����5:�׶�1:298:277:����:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����5:�׶�1:����:298:277:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "292:282|286:289|281:295|274:302";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[274:302]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[274:302]:�ɹ�");
    
    //5-2
    Delay_With_Terminate_Check(60, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����5:�׶�1:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 226) <= 1 && abs(posY - 277) <= 1)
                {
                    api_log_MsgDebug("����5:�׶�2:����:226:277:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����5:�׶�2:����:226:277:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "221:271|216:266|212:262|205:256";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[205:256]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[205:256]:�ɹ�");

    //5-3
    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����5:�׶�3:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 230) <= 1 && abs(posY - 206) <= 1)
                {
                    api_log_MsgDebug("����5:�׶�3:����:230:206:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����5:�׶�3:����:230:206:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "236:202|245:197|251:193|258:192";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[258:192]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[258:192]:�ɹ�");

    //5-4
    Delay_With_Terminate_Check(20, luaman_task, dmobj);

    status = beat_cpy5_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����5:�׶�4:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 289) <= 1 && abs(posY - 205) <= 1)
                {
                    api_log_MsgDebug("����5:�׶�4:����:289:205:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����5:�׶�3:����:289:205:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "296:213|303:219";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[303:219]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[303:219]:�ɹ�");

    //5-5
    status = beat_cpy5_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("����5:�׶�5:���:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    //TODO:ʱ��С��10��ʱ��0
    Delay_With_Terminate_Check(20, luaman_task, dmobj);
    
    //�ȴ�����ȥ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("0");
        Delay(200);
        dmobj->KeyUpChar("0");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(1000);
        api_log_MsgDebug("��ͼ:������:����:ʧ��");
    }

    pathofposxys = "460:460";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[460:460]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[460:460]:�ɹ�");

    api_log_MsgDebug("����5:�ɹ�:����:��һ��");
    return SNAIL_ERRNO_SUCCESS;
}

static int singlecpy1_Fighttimeout(Idmsoft* dmobj, void* ctxt, int waittime, int fighttime)
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

    for (idx = 0; idx < 60; idx++)
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


static int beat_cpy6_phase1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy1_fight_atpoint_1();

    api_log_MsgDebug("��1:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��1::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy6_phase2(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(20);
            dmobj->KeyUpChar("3");
            Delay(20);
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_cove.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ǰ����Ѩ����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ǰ����Ѩ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        deltaTime = difftime(time(NULL), lastChckTime);
        if (deltaTime > (120 * 1.0))
        {
            api_log_MsgDebug("��ǰ����Ѩ����:���:��ʱ");
            break;
        }
        
        api_log_MsgDebug("��ǰ����Ѩ����:����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("����6:�׶�2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy6_phase3(Idmsoft* dmobj, void* ctxt)
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
    double deltaTime = 0.0;
    time_t lastChckTime = time(NULL);

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������˹", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������˹", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc8.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������˹", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������˹:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("��������˹:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����6:�׶�3:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����6:�׶�3:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    api_log_MsgDebug("����6:�׶�3:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy6(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    int deltaX = deltaatpos_single_cpy6[0].x;
    int deltaY = deltaatpos_single_cpy6[0].y;

    if (tasknr)
    {
        goto Label_next_cpy_task;
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    pathofposxys = "347:123|346:116";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[346:116]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[346:116]:�ɹ�");

    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�봫��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�봫��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�봫��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȵ�", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("�����ȵ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�����ȵ�:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȵ�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�����ȵ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�����ȵ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ���ͼ
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "�����ȵ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:�����ȵ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:�����ȵ�:����:ʧ��");
        Delay(1000);
    }

    pathofposxys = "102:118|103:125|103:132|102:139|101:147|99:153|96:158|92:160";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[92:160]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[92:160]:�ɹ�");

    //////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�ƶ�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ƶ�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׶�����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("���׶�����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("���׶�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���׶�����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���׶�����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("���׶�����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ���ͼ
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׶�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���׶�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���׶�����:����:ʧ��");
        Delay(1000);
    }

    pathofposxys = "474:43|477:54|478:65|478:78|478:87|477:97|478:108|480:117|482:126|483:134";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[483:134]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[483:134]:�ɹ�");

    //////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

    //�ȴ�10��
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy6[0].x;
    deltaY = deltaatpos_single_cpy6[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

Label_next_cpy_task:
    pathofposxys = (0 == tasknr)? "315:379|321:383|327:394|335:385|341:382|346:379|350:375|355:373|360:372|365:372" :
        "327:394|335:385|341:382|346:379|350:375|355:373|360:372|365:372";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    //////////////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�볡����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�볡����:����:ʧ��");
        Delay(500);
    }

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    //���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�볡����:���:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for (;;)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����������:���:�ɹ�");
                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("�볡����:���::ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�볡����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�볡����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

Label_NPC_next_retry:
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�볡����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�볡����:����:ʧ��");
        Delay(500);
    }

    deltaX = deltaatpos_single_cpy6[1].x;
    deltaY = deltaatpos_single_cpy6[1].y;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:����:������:���:����");
            goto Label_NPC_next_retry;
        }
        
        api_log_MsgDebug("ȷ��:����:ʧ��");
        Delay(500);
    }

    //���ȷ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:���:������:���:����");
            goto Label_NPC_next_retry;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ��̶�ʱ��
    Delay_With_Terminate_Check_Alram(single_cpy_wait_seconds, luaman_task, dmobj);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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
    
    deltaX = deltaatpos_single_cpy6[2].x;
    deltaY = deltaatpos_single_cpy6[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //////////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������΢����ħ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("������΢����ħ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("������΢����ħ:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc6.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "������΢����ħ", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("������΢����ħ:���:�ɹ�");
            break;
        }
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("������΢����ħ:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    //���
    code = beat_cpy6_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��1:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��1:�ɹ�");

    code = beat_cpy6_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��2:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��2:�ɹ�");

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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
    
    deltaX = deltaatpos_single_cpy6[3].x;
    deltaY = deltaatpos_single_cpy6[3].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //////////////////////////////////////////////////////////////////////////////////////
    //��N�Ի���������붴Ѩ����
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���붴Ѩ", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("���붴Ѩ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���붴Ѩ:����:ʧ��");
        Delay(500);
    }
    
    //���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc7.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���붴Ѩ", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���붴Ѩ:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("���붴Ѩ:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //��鵱ǰ����,������겻��109:246������Ի�ʧ��,��һ�ػ�û�д��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 109) <= 2 && abs(posY - 246) <= 2)
                {
                    api_log_MsgDebug("����:109:246:����:�ɹ�");
                    break;
                }
            }
        }

        api_log_MsgDebug("����:109:246:����:ʧ��");
        Delay(500);
    }

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);

    pathofposxys = "97:244";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[97:244]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[97:244]:�ɹ�");

    //���
    code = beat_cpy6_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        return code;
    }

    api_log_MsgDebug("�ȴ�:15��:��ʼ");
    
    Delay_With_Terminate_Check_Alram(15, luaman_task, dmobj);
    
    api_log_MsgDebug("�ȴ�:15��:����");

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
        
    //�ȴ���ͼ
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׶�����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���׶�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���׶�����:����:ʧ��");
        Delay(1000);
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_1();

    api_log_MsgDebug("��1:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��1::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase2(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_2();

    api_log_MsgDebug("��2:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��2::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase3(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_3();

    api_log_MsgDebug("��3:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��3::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase4(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_4();

    api_log_MsgDebug("��4:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��4::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase5(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_5();

    api_log_MsgDebug("��5:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��5::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase6(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_6();

    api_log_MsgDebug("��6:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��6::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase7(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_7();

    api_log_MsgDebug("��7:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��7::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase8(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_8();

    api_log_MsgDebug("��8:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��8::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase9(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_9();

    api_log_MsgDebug("��9:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��9::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase10(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_10();

    api_log_MsgDebug("��10:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��10::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase11(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy7_fight_atpoint_11();

    api_log_MsgDebug("��11:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }
    
    api_log_MsgDebug("��11::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase12(Idmsoft* dmobj, void* ctxt)
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
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("��12::���:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("���������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("���������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("���������:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("���������:����:ʧ��");
        Delay(500);
    }

    flag = 0;
    
    //��ʼ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 50; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        if (0 == flag)
        {
            code = dmobj->FindPic(535, 49, 768, 160, BMPSIGNLECPY1_DIR"hiborun1.bmp", "101010", 0.9, 0, &pposX, &pposY);
            if (-1 == code)
            {
                dmobj->KeyDown(192);
                Delay(200);
                dmobj->KeyUp(192);
                Delay(200);
                
                dmobj->KeyDownChar("~");
                Delay(200);
                dmobj->KeyUpChar("~");
                Delay(200);

                Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

                dmobj->KeyDownChar("w");
                Delay(200);
                dmobj->KeyUpChar("w");
                Delay(200);

                flag = 1;
            }
        }
        
        if (flag)
        {
            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("���������:����ʱ��:��ɫ:���:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("���������:����ʱ��:����:���:����");
            Delay(500);
        }
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }
    
    api_log_MsgDebug("��12::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy7_phase13(Idmsoft* dmobj, void* ctxt)
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
    const char* fontcolors[] = {"ffffff-101010", "00ff00-101010", "ff3300-101010"};
    const char* fontcolor = fontcolors[0];
    int flag = 0;
    
    api_log_MsgDebug("��13::���:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ά��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[0];
            
            api_log_MsgDebug("�����ά��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ά��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[1];
            
            api_log_MsgDebug("�����ά��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-MingZhi2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ά��", "ff3300-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            fontcolor = fontcolors[2];
            
            api_log_MsgDebug("�����ά��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("�����ά��:����:ʧ��");
        Delay(500);
    }

    //��ʼ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        for (idx = 0; idx < 20; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(15);
            dmobj->KeyUpChar("3");
            Delay(15);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-TiShi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ʱ��", "fdb602-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����7:�׶�13:�ȴ�ʱ��:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("����7:�׶�13:�ȴ�ʱ��:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 60; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }
    
    api_log_MsgDebug("��13::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

int signle_copy7(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    char* randtime = NULL;
    int deltaX = deltaatpos_single_cpy7[0].x;
    int deltaY = deltaatpos_single_cpy7[0].y;
    int retrytimes = 0;
    int flags = 0;
    int reclick = 1;

    if (tasknr)
    {
        goto Label_next_cpy7_task;
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    pathofposxys = "347:123|346:116";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[346:116]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[346:116]:�ɹ�");

    dmobj->MoveTo(5, 5);
    Delay(200);

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�봫��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�봫��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�봫��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ɽ��", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("����ɽ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����ɽ��:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�����ȵ�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�����ȵ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�����ȵ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 130) <= 1 && abs(posY - 304) <= 1)
                {
                    api_log_MsgDebug("����7:����:130:304:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����7:����:130:304:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "126:297|123:289|122:284|125:278|131:270|139:265|143:259|143:251|141:245";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[141:245]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[141:245]:�ɹ�");

    reclick = 1;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]�ƶ���DK������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("[DAS]�ƶ���DK������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[DAS]�ƶ���DK������:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        if (reclick)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
        }
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DiTUMingZhi2.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "����ɽ�ص���Ԩ", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("��ͼ:����ɽ�ص���Ԩ:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK������1" : "DK������2";
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, randtime, "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("%s:���:�ɹ�", randtime);

            dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-talk3.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]�ƶ���DK������", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                reclick = 1;
                
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("[DAS]�ƶ���DK������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            else
            {
                reclick = 0;
                api_log_MsgDebug("[DAS]�ƶ���DK������:����:ʧ��");
            }
            
            continue;
        }

        reclick = 1;
        
        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        lposX = lposX2;
        lposY = lposY2;
        
        api_log_MsgDebug("%s:���:ʧ��, lposX2:%ld, lposY2:%ld", randtime, lposX2, lposY2);
        
        Delay(500);
    }
    
    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[0].x;
    deltaY = deltaatpos_single_cpy7[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    pathofposxys = "291:284|293:270|294:262|294:255";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[294:255]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[294:255]:�ɹ�");

Label_next_cpy7_task:
    //��p���˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(500);
        dmobj->KeyUpChar("p");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�˳�����:����:ʧ��");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
        Delay(500);
    }

    //�����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����p
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("p");
        Delay(200);
        dmobj->KeyUpChar("p");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-auxiend.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 == code)
            {
                api_log_MsgDebug("�˳�����:�ر�:�ɹ�");
                break;
            }
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�˳�����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��]�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("��]�볡����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��]�볡����:����:ʧ��");
        Delay(500);
    }

    retrytimes = 0;
    flags = 1;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        if (flags)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(200);
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua2.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "��]�볡����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX2.lVal;
            lposY = pposY2.lVal;
            flags = 1;
            
            api_log_MsgDebug("��]�볡����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            continue;
        }

        flags = 0;
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("����������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                check_alarm(dmobj, ctxt);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "����������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("����������:���:�ɹ�");

                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;
                
                api_log_MsgDebug("����������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
            return SNAIL_ERRNO_NOMORE;
        }
        
        api_log_MsgDebug("��]�볡����:���:����:����:%d", retrytimes);
        Delay(500);

        retrytimes++;
        if (retrytimes > 2)
        {
            api_log_MsgDebug("��]�볡����:���:�ɹ�");
            break;
        }
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua3.txt");
        code = dmobj->FindStrFast(385, 27, 1279, 826, "Ƭ(����)", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("Ƭ(����):����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("Ƭ(����):����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
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
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
        
        api_log_MsgDebug("ȷ��:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    /*for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 343) <= 1 && abs(posY - 303) <= 1)
                {
                    api_log_MsgDebug("����7:����:343:303:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����7:����:343:303:����:��ȡ:ʧ��");
        Delay(500);
    }*/
    //�ȴ���ͼ
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DiTUMingZhi3.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���׵���Ƭ", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���׵���Ƭ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:���׵���Ƭ:����:ʧ��");
        Delay(1000);
    }

    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(200);
    dmobj->MiddleClick();
    LUAMAN_WAIT_MS(200);
        
    Delay_With_Terminate_Check_Alram(10, luaman_task, dmobj);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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
    
    deltaX = deltaatpos_single_cpy7[1].x;
    deltaY = deltaatpos_single_cpy7[1].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //���
    code = beat_cpy7_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��1:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��1:�ɹ�");
    
    //�һ�ɫ�����־��������ҵ��˵����ɫ�����־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���־�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���־�����:����:ʧ��");
        Delay(500);
    }

    //������־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[0].x, lposY + single_cpy7_ips_offset[0].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���־�����:���:�ɹ�");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("���־�����:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    code = beat_cpy7_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��2:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��2:�ɹ�");

    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[2].x;
    deltaY = deltaatpos_single_cpy7[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase3(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��3:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��3:�ɹ�");
    
    //�һ�ɫ�����־��������ҵ��˵����ɫ�����־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���־�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���־�����:����:ʧ��");
        Delay(500);
    }

    //������־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[1].x, lposY + single_cpy7_ips_offset[1].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���־�����:���:�ɹ�");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("���־�����:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    code = beat_cpy7_phase4(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��4:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��4:�ɹ�");
    
    //////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[3].x;
    deltaY = deltaatpos_single_cpy7[3].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase5(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��5:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��5:�ɹ�");
    
    //�һ�ɫ�����־��������ҵ��˵����ɫ�����־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���־�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���־�����:����:ʧ��");
        Delay(500);
    }

    //������־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[2].x, lposY + single_cpy7_ips_offset[2].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���־�����:���:�ɹ�");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("���־�����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    code = beat_cpy7_phase6(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��6:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��6:�ɹ�");
    
    //////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[4].x;
    deltaY = deltaatpos_single_cpy7[4].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase7(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��7:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��7:�ɹ�");

    pathofposxys = "221:303";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[221:303]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[221:303]:�ɹ�");
    
    //�һ�ɫ�����־��������ҵ��˵����ɫ�����־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���־�����:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���־�����:����:ʧ��");
        Delay(500);
    }

    //������־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_ips_offset[3].x, lposY + single_cpy7_ips_offset[3].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���־�����", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���־�����:���:�ɹ�");
            break;
        }

        lposX = pposX2.lVal;
        lposY = pposY2.lVal;
    
        api_log_MsgDebug("���־�����:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    code = beat_cpy7_phase8(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��8:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��8:�ɹ�");

    ////////////////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[5].x;
    deltaY = deltaatpos_single_cpy7[5].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    code = beat_cpy7_phase9(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��9:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��9:�ɹ�");

    pathofposxys = "183:248";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[183:248]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }
    
    code = beat_cpy7_phase10(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��10:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��10:�ɹ�");

    pathofposxys = "182:256|183:264";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[183:264]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    code = beat_cpy7_phase11(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��11:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��11:�ɹ�");
    
    ////////////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[6].x;
    deltaY = deltaatpos_single_cpy7[6].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);
    
    //�һ�ɫ�Ľ��ʯ
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���ʯ:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���ʯ:����:ʧ��");
        Delay(500);
    }

    //������־�����(font-DuiHua4.txt)�����λ��
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + single_cpy7_jjs_offset[0].x, lposY + single_cpy7_jjs_offset[0].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua5.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���ʯ", "ffff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���ʯ:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
    
        api_log_MsgDebug("���ʯ:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[7].x;
    deltaY = deltaatpos_single_cpy7[7].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //���
    code = beat_cpy7_phase12(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��12:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��12:�ɹ�");

    ////////////////////////////////////////////////////////////////////////////////////////////
    //��һ�¼��ż���������
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    //��M���ҹ����������λ��2
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
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

    deltaX = deltaatpos_single_cpy7[8].x;
    deltaY = deltaatpos_single_cpy7[8].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    dmobj->MoveTo(5, 5);
    Delay(100);
    dmobj->KeyDownChar("m");
    Delay(300);
    dmobj->KeyUpChar("m");
    Delay(300);

    //���
    code = beat_cpy7_phase13(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��13:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���:��13:�ɹ�");
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�Ի�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�Ի�:����:�ɹ�:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        api_log_MsgDebug("�Ի�:����:�ɹ�:ʧ��");
        Delay(500);
    }

    //���'�������档'
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�˳�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-DuiHua6.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�˳�����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("�˳�����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�������292:269
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 292) <= 1 && abs(posY - 269) <= 1)
                {
                    api_log_MsgDebug("����7:�˳�:292:269:�ɹ�");
                    break;
                }
                else
                {
                    
                }
            }
        }

        api_log_MsgDebug("����7:�˳�:292:269:����:��ȡ:ʧ��");
        Delay(500);
    }

    pathofposxys = "294:255";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_walkcpy1(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[294:255]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[294:255]:�ɹ�");
    
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy_foolish_phase1(Idmsoft* dmobj, void* ctxt)
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
    struct fighttimeinfo fti[] = signlecpy_foolish_fight_atpoint_1();
    char bmpFileName[255] = {0};
    SYSTEMTIME curTimeStamp;
    
    api_log_MsgDebug("��1:���:��ʼ");
    
    for (idx = 0; idx < pirate_dim(fti); idx++)
    {
        (void)singlecpy1_Fighttimeout(dmobj, ctxt, fti[idx].wait_time, fti[idx].fight_time);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    GetLocalTime(&curTimeStamp);
    _snprintf(bmpFileName, sizeof(bmpFileName) - 1, "cap\\single\\%04d%02d%02d-%02d%02d%02d.bmp",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
        curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond);
    dmobj->Capture(0, 27, 1279, 826, bmpFileName);
    
    api_log_MsgDebug("��1::���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int beat_cpy_foolish_phase2(Idmsoft* dmobj, void* ctxt)
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
    char bmpFileName[255] = {0};
    SYSTEMTIME curTimeStamp;
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        dmobj->KeyDownChar("u");
        Delay(50);
        dmobj->KeyUpChar("u");
        Delay(50);
        
        for (idx = 0; idx < 60; idx++)
        {
            dmobj->KeyDownChar("3");
            Delay(10);
            dmobj->KeyUpChar("3");
            Delay(10);
        }

        //�����Ͻǵĵȴ�ʱ��
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-foolish2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����ȴ�", signlecpy_foolish_diewait_font_color, 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("�׶�2:����ȴ�:��ȡ:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�׶�2:����ȴ�:��ȡ:ʧ��:����:���");
        Delay(500);
    }

    for (idx = 0; idx < 30; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(30);
        dmobj->KeyUpChar("space");
        Delay(30);
    }

    GetLocalTime(&curTimeStamp);
    _snprintf(bmpFileName, sizeof(bmpFileName) - 1, "cap\\single\\%04d%02d%02d-%02d%02d%02d.bmp",
        curTimeStamp.wYear, curTimeStamp.wMonth, curTimeStamp.wDay,
        curTimeStamp.wHour, curTimeStamp.wMinute, curTimeStamp.wSecond);
    dmobj->Capture(0, 27, 1279, 826, bmpFileName);
    
    api_log_MsgDebug("�׶�2:���:���");
    return SNAIL_ERRNO_SUCCESS;
}


int signle_copy_foolish(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    int index = 0;
    time_t start_tmstmp;
    time_t end_tmstmp;
    double time_delta = 0.0;
    int deltaX = 0;
    int deltaY = 0;
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //��M���ҹ����������λ��2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[0].x;
    deltaY = deltaatpos_single_cpy_foolish[0].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�볡����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�볡����:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);
    
    //���
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�볡����:���:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("��������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

            //�ص���������
            for_timeout_check_start(luaman_task)
            {
                Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
                dmobj->MoveTo(lposX2, lposY2);
                Delay(300);
                dmobj->LeftClick();
                Delay(300);
                dmobj->MoveTo(5, 5);
                Delay(300);

                dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("��������:���:�ɹ�");
                    return SNAIL_ERRNO_NOMORE;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
                
                api_log_MsgDebug("��������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            for_timeout_check_end();
            
            return SNAIL_ERRNO_NOMORE;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�볡����:���:sʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�볡����:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�볡����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    for_timeout_check_end();
    
    //////////////////////////////////////////////////////////////////////////////////////////////////
    dmobj->MoveTo(5, 5);
    Delay(200);

Label_next_retry:
    
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTSIGNLECPY1_DIR"font-foolish1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "?????", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("?????:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("?????:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    //��� 50:444 -> 50:426
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�Ի�:ȷ��:����:��Χ,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("?????:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    //���ȷ��
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        code = check_copyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:���:�������³�����:���¶Ի�");
            goto Label_next_retry;
        }
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("ȷ��:���:��Χ:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    for_timeout_check_end();

    //�ȴ�����
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - 58) <= 2 && abs(posY - 70) <= 2)
                {
                    api_log_MsgDebug("58:70:�ﵽ:�ɹ�");
                    break;
                }
            }
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("58:70:����:��ȡ:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //��M���ҹ����������λ��2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[1].x;
    deltaY = deltaatpos_single_cpy_foolish[1].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //��ʼ���
    code = beat_cpy_foolish_phase1(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��1:ʧ��, code:%d", code);
        return code;
    }
    //////////////////////////////////////////////////////////////////////////

    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);
    
    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);

    //��M���ҹ����������λ��2
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("m");
        Delay(300);
        dmobj->KeyUpChar("m");
        Delay(300);

        dmobj->SetDict(0, FONTSIGNLECPY1_DIR"font-vip.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������", "b7b7b7-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        
        api_log_MsgDebug("�������:����:ʧ��");
        Delay(500);
    }
    for_timeout_check_end();
    
    deltaX = deltaatpos_single_cpy_foolish[2].x;
    deltaY = deltaatpos_single_cpy_foolish[2].y;
    
    dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(300);
    
    dmobj->KeyDownChar("-");
    Delay(300);
    dmobj->KeyUpChar("-");
    Delay(300);

    Delay_With_Terminate_Check_Alram(1, luaman_task, dmobj);

    //���
    code = beat_cpy_foolish_phase2(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgDebug("���:��2:ʧ��, code:%d", code);
        return code;
    }

    api_log_MsgDebug("���˽�:����:���");
    return SNAIL_ERRNO_NOMORE;
}

static void callend(){}
