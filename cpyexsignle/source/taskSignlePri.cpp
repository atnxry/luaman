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
#include "taskSignlePri.h"
#include "taskSignleCpy1Pri.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"
#include "curl.h"


#define CHECK_CONN_LOST_TIME    (5)
#define FONTSIGNLE_DIR "font\\exsignle\\"
#define BMPSIGNLE_DIR "bmp\\exsignle\\"

struct gabage_info
{
    const char* name;
    const char* color_offset;
};

///////////////////////////////////////////////////////////////////////////////
//�������,ÿ����1������,������һ��
//!!! '�ӻ��̵�'���뵽�ֿ��ļ�font-clrpkg1.txt��
//!!!��Ҫ!!!:�������������Ƽ��뵽�ֿ��ļ�font-clrpkg2.txt��
//!!! ���ֿ��ļ�font-clrpkg1.txt��font-clrpkg2.txt���ŵ�product/font/signle/��
static gabage_info usrdef_gabage_info[] = 
{
    //{"����1", "00ff00-010101"}, //һ�б�ʾһ������
    //{"����2", "00ff00-010101"},
    //TODO:�ο�����1������2д,'//'��ʾע��
    {"Ҫ��ְҵ", "ffffff-010101"},
    {"��������", "ffffff-010101"},
    {"��������", "c1c1ff-010101"},
    {"��������", "ffe08c-010101"},
    
    
};
///////////////////////////////////////////////////////////////////////////////
static int signle_checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int signle_checklife(Idmsoft* dmobj, void* ctxt, int recusive)
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

    {
        //return ERRNO_SUCCESS;
    }
    
    dmobj->SetDict(0, FONTSIGNLE_DIR"font-blood.txt");
    strBBlood = dmobj->Ocr(627, 778, 859, 788, "ffffff-101010", 0.9);
    dmobj->ClearDict(0);
    if (strBBlood == CString(""))
    {
        api_log_MsgDebug("��Ѫ��:��ȡ:�쳣, groupid:%d, hwnd:%ld", luaman_config->groupid, hwnd);
        return SNAIL_ERRNO_INGORE;
    }

    cstr = strBBlood.GetBuffer(BUFFER_MAX);
    strBBlood.ReleaseBuffer();

    sz = SplitString(strBBlood, '/', bBlood);
    if (2 == sz)
    {
        bBloodAva = atol(bBlood.GetAt(0));
        bBloodTot = atol(bBlood.GetAt(1));
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        api_log_MsgDebug("Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
            hwnd, bBloodAva, bBloodTot);
    }

    if (bBloodAva > blueThreshold)
    {
        return SNAIL_ERRNO_SUCCESS;
    }

    if (bBloodAva > 0)
    {
        dmobj->KeyDownChar("f3");
        LUAMAN_WAIT_MS(200);
        dmobj->KeyUpChar("f3");
        LUAMAN_WAIT_MS(200);

        return SNAIL_ERRNO_SUCCESS;
    }

    api_log_MsgDebug("Ѫ��:���:����,�ȴ����ֽ�������");

    signle_checkkeystate(dmobj, ctxt);

    if (recusive)
    {
        return SNAIL_ERRNO_AGAIN;
    }
    
    //�ȴ�������������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:����:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("Ѫ��:���:����:����:����:ʧ��");
        Delay(500);
    }
    
    //�������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-chcklife.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-000000", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("Ѫ��:���:����:����:���,�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("Ѫ��:���:����:����:���,ʧ��,lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //�ȴ�����վ����
    for (;;)
    {
        code = signle_checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("����:վ��:���");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    return SNAIL_ERRNO_REALIVE;
}

int signle_pri_walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
                (void)signle_checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = signle_checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)signle_checkkeystate(dmobj, ctxt);
                
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

                (void)signle_checkkeystate(dmobj, ctxt);
                
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
        code = signle_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:Ѫ��:���:����:����");
            return code;
        }
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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

static int signlecopy1_pre_join2team(Idmsoft* dmobj, void* ctxt)
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

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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
        
        //���Ѫ��
        code = signle_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:��Ӻ���:����:����");
            return code;
        }

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
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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

        //���Ѫ��
        code = signle_checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("���:����:Ƶ��:����:����:����");
            return code;
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

                dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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

                //���Ѫ��
                code = signle_checklife(dmobj, ctxt, 0);
                if (SNAIL_ERRNO_REALIVE == code)
                {
                    api_log_MsgDebug("���:����:��Ա:����:����:����:����");
                    return code;
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

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-inviteheil2team-1.txt");
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

static int signlecopy1_pre(Idmsoft* dmobj, void* ctxt)
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
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    dmobj->KeyDownChar("f2");
    Delay(300);
    dmobj->KeyUpChar("f2");
    Delay(300);

    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|460:89|467:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[467:89]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[467:89]:�ɹ�");

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(300);

    //����ͼ�ǲ��Ǳ�ɴ��;������㳡
    i = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���;������㳡", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���;������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        i++;
        if (i == 10)
        {
            dmobj->KeyDownChar("d");
            Delay(300);
            dmobj->KeyUpChar("d");
            Delay(300);

            i = 0;
            api_log_MsgDebug("��ͼ:���;������㳡:����:d��:�ɹ�");
        }
        
        api_log_MsgDebug("��ͼ:���;������㳡:����:ʧ��");
        Delay(1000);
    }

    i = 0;
    
    pathofposxys = "370:256|360:256|351:256|341:256|334:256|324:252";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[324:252]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[324:252]:�ɹ�");

    dmobj->KeyDownChar("=");
    Delay(1000);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyDownChar("0");
    Delay(1000);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    pathofposxys = "314:245|308:237|307:227|307:221|307:210";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[307:210]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[307:210]:�ɹ�");

    return SNAIL_ERRNO_SUCCESS;
}

static int signlecopy1_exitgroup(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int groupid = luaman_config->groupid;
    int code = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    VARIANT pposX3, pposY3;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX3 = 0, lposY3 = 0;
    int retrynr = 0;
    
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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�˳�����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        retrynr++;
        if (retrynr > 10)
        {
            api_log_MsgDebug("�˳�����:����:ʧ��:��ʱ");
            return ERRNO_SUCCESS;
        }
        
        api_log_MsgDebug("�˳�����:����:ʧ��");
        Delay(500);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX + 5, lposY + 5);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
            
            api_log_MsgDebug("ȷ��:����:�ɹ�,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Delay(500);
        api_log_MsgDebug("ȷ��:����:ʧ��");
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
        Delay(200);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ȷ��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ȷ��:���:ʧ��,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }

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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ESC:�˳�����:����:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("ESC:�˳�����:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

int signle_pri_clickhome(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = luaman_config->cleanPackageX;
    int code = 0;
    //VARIANT pposX, pposY;
    VARIANT pposX1, pposY1;
    VARIANT pposX2, pposY2;
    //long lposX = 0, lposY = 0;
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

    //api_log_MsgDebug("cleanPackageX:0x%x", cleanPackageX);
    
    //���ұ�����ǩ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        dmobj->KeyDownChar("i");
        Delay(300);
        dmobj->KeyUpChar("i");
        Delay(300);
        
        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("�ֿ�:����:�ɹ�");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;
    
    api_log_MsgDebug("����:�����, lineX:%ld, lineY:%ld", lineX, lineY);

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

    api_log_MsgDebug("����:�����:׼��:����");

    //����ÿ�����������б����
    for (lndex = 0; lndex < 5; lndex++)
    //for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

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

                api_log_MsgDebug("����:����:%d:����:[%d,%d]:����:[%ld, %ld]",
                    lndex, index, jndex, posX, posY);
                
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-home.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "���ؾ���", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX1 = pposX1.lVal;
                    lposY1 = pposY1.lVal;

                    Delay(200);
                    dmobj->RightClick();
                    Delay(200);

                    api_log_MsgDebug("���ؾ���:����:�ɹ�, lposX1:%ld, lposY1:%ld", lposX1, lposY1);

                    return SNAIL_ERRNO_SUCCESS;
                }
            }
        }
    }

    return SNAIL_ERRNO_NOT_EXIST;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//���ؿ�����:����ȥ��ǰ���ע��,�ر���ע��

//���˽ڸ���,�������˽ڸ����Ͳ��ܴ�1~6��;��֮��Ȼ
//#define SINGLE_FOOLISH_DAY

#ifndef SINGLE_FOOLISH_DAY
    //#define SIGNLE_SEGMENT12_ONLY
    //#define SIGNLE_SEGMENT3
    //#define SIGNLE_SEGMENT4
    //#define SIGNLE_SEGMENT124
    //#define SIGNLE_SEGMENT12345
    //#define SIGNLE_SEGMENT45
    //#define SIGNLE_SEGMENT123456
    //#define SIGNLE_SEGMENT6
    //#define SIGNLE_SEGMENT7
    //#define SIGNLE_SEGMENT1247
    #define SIGNLE_SEGMENT123457
    //#define SIGNLE_SEGMENT1234567
    //#define SIGNLE_SEGMENT345
    //#define SIGNLE_SEGMENT3457
    //#define SIGNLE_SEGMENT457
    //#define SIGNLE_SEGMENT347
    //#define SIGNLE_SEGMENT12347
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//!!! ���µ����ݲ�Ҫ��
#ifdef SIGNLE_SEGMENT12_ONLY
    #define SIGNLE_SEGMENT12
    #define LARGE_SPORT_SQUARE
#endif

#if defined(SIGNLE_SEGMENT124) || defined(SIGNLE_SEGMENT1247)
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT12345_124
#endif

#ifdef SIGNLE_SEGMENT3457
    #define SIGNLE_SEGMENT345
#endif

#ifdef SIGNLE_SEGMENT345
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT45
#endif

#ifdef SIGNLE_SEGMENT12347
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT347
#endif

#ifdef SIGNLE_SEGMENT347
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT7
#endif

#ifdef SIGNLE_SEGMENT457
    #define SIGNLE_SEGMENT45
#endif

#ifdef SIGNLE_SEGMENT45
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT5
#endif

#if defined(SIGNLE_SEGMENT123456) || defined(SIGNLE_SEGMENT1234567)
    #define SIGNLE_SEGMENT12345
    #define SIGNLE_SEGMENT6
#endif

#if defined(SIGNLE_SEGMENT12345) || defined(SIGNLE_SEGMENT123457)
    #define SIGNLE_SEGMENT12
    #define SIGNLE_SEGMENT12345_124
    #define SIGNLE_SEGMENT3
    #define SIGNLE_SEGMENT4
    #define SIGNLE_SEGMENT5
#endif

#if defined(SIGNLE_SEGMENT457) || defined(SIGNLE_SEGMENT3457) || defined(SIGNLE_SEGMENT1247) || defined(SIGNLE_SEGMENT123457) || defined(SIGNLE_SEGMENT1234567)
    #define SIGNLE_SEGMENT7
#endif

static int signleBuyBlue(Idmsoft* dmobj, void* ctxt)
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

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //���֮�����"�̵�"����
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-shop.txt");
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
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX1 + 160, lposY1 + 74);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
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
    code = dmobj->SendString(hwnd, "3000");
    if (1 != code)
    {
        api_log_MsgError("�̵�:���:3000:����:ʧ��");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("����:���:3000:����:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX2, lposY2);
        Delay(500);
        dmobj->LeftDown();
        Delay(500);
        dmobj->LeftUp();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-okcancel.txt");
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

    Check_Terminate_Thread(luaman_task, dmobj);
    check_alarm(dmobj, ctxt);
    
    api_log_MsgDebug("����:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int signlecopy_sellgabage(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int cleanPackageX = 0x6;//luaman_config->cleanPackageX;//Ĭ������1,2��
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
    int idx = 0;
    
    api_log_MsgDebug("���:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);

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

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    lineX = boxPosX0 + deltaX + 3 - 16;
    lineY = boxPosY0 + deltaY + 3 - 19;

    api_log_MsgDebug("����:�����, lineX:%ld, lineY:%ld", lineX, lineY);

    for (index = 0; index < 5; index++)
    {
        posY = lineY + index * deltaY;
        
        for (jndex = 0; jndex < 10; jndex++)
        {
            posX = lineX + jndex * deltaX;
            
            boxPosXYs[index * 10 + jndex].x = posX;
            boxPosXYs[index * 10 + jndex].y = posY;

            //api_log_MsgDebug("boxPosXYs[%d][%d] = [%ld, %ld]", index, jndex, posX, posY);
        }
    }

    for (lndex = 0; lndex < 5; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }

        //�㿪��ǩҳ
        dmobj->MoveTo(labelPosXY[lndex].x, labelPosXY[lndex].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        for (index = 0; index < 5; index++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            for (jndex = 0; jndex < 10; jndex++)
            {
                posX = boxPosXYs[index * 10 + jndex].x;
                posY = boxPosXYs[index * 10 + jndex].y;

                dmobj->MoveTo(posX, posY);
                Delay(250);

                for (idx = 0; idx < sizeof(usrdef_gabage_info)/sizeof(usrdef_gabage_info[0]); idx++)
                {
                    dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg2.txt");
                    code = dmobj->FindStrFast(0, 27, 1279, 826, usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset, 0.9, &pposX1, &pposY1);
                    dmobj->ClearDict(0);
                    if (0 != code)
                    {
                        continue;
                    }

                    Delay(200);
                    dmobj->RightClick();
                    Delay(300);
                    dmobj->KeyDownChar("enter");
                    Delay(300);
                    dmobj->KeyUpChar("enter");
                    Delay(300);

                    api_log_MsgDebug("���:����:%d:����:[%d:%d]:����:%d:����:%s:��ɫ:%s",
                        lndex, index, jndex, idx,
                        usrdef_gabage_info[idx].name, usrdef_gabage_info[idx].color_offset);
                    break;
                }
            }
        }
    }

    //���� 2023/5/17 22:00:00
    (void)signleBuyBlue(dmobj, ctxt);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("���:���");
    return SNAIL_ERRNO_SUCCESS;
}


static int signlecopy_cleanpackage(Idmsoft* dmobj, void* ctxt)
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
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|460:89|467:89";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[467:89]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[467:89]:�ɹ�");

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(300);

    //����ͼ�ǲ��Ǳ�ɴ��;������㳡
    i = 0;
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "���;������㳡", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:���;������㳡:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        i++;
        if (i == 10)
        {
            dmobj->KeyDownChar("d");
            Delay(300);
            dmobj->KeyUpChar("d");
            Delay(300);

            i = 0;
            api_log_MsgDebug("��ͼ:���;������㳡:����:d��:�ɹ�");
        }
        
        api_log_MsgDebug("��ͼ:���;������㳡:����:ʧ��");
        Delay(1000);
    }

    i = 0;
    
    pathofposxys = "370:256|360:256|351:256|341:256|334:256|324:252|317:255|306:255|296:255|288:255|280:255|271:255|262:256|252:255|245:255|237:256|229:256|221:256|221:264|221:267|220:273|219:278|216:283|217:287";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[217:287]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[217:287]:�ɹ�");
    
    //��n
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ӻ��̵�", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("�ӻ��̵�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ӻ��̵�:����:ʧ��");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-clrpkg1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ӻ��̵�", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ӻ��̵�:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
        
        api_log_MsgDebug("�ӻ��̵�:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    return signlecopy_sellgabage(dmobj, ctxt);
}

int signle_pri_postwalk(Idmsoft* dmobj, void* ctxt)
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
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int tasknrref = 0;
    char* randtime = NULL;

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    pathofposxys = "292:49|301:52|308:54|305:48|304:43|304:40";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[304:40]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[304:40]:�ɹ�");

    return SNAIL_ERRNO_SUCCESS;
}

int signle_pri_copy(Idmsoft* dmobj, void* ctxt, int tasknr)
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
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    int i = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int tasknrref = 0;
    char* randtime = NULL;

#ifdef SINGLE_FOOLISH_DAY
    /*
      ���˽ڸ���
      1.Ӧ�õ���Ūһ��������,Ϊ�˿�����ɲ�Ӧ��,ֱ��ͨ����ķ�ʽ�Ž���
      2.�����ʱ��ֱ�ӱ�һ����֮ǰ1~6���������Ķ����Ƴ���汾 2023/4/1 10:04:11
    */
    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy_foolish(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����:���˽�:����:���:tasknrref:%d", tasknrref);
            return SNAIL_ERRNO_SUCCESS;
        }

        api_log_MsgDebug("����:���˽�:����:����:tasknrref:%d", tasknrref);
        Delay(500);
    }
#endif

    dmobj->KeyDownChar("f2");
    Delay(500);
    dmobj->KeyUpChar("f2");
    Delay(500);

#ifdef SIGNLE_SEGMENT12
    #ifdef SIGNLE_SEGMENT12345_124
        status = signlecopy1_pre(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("����1:ǰ��:ʧ��:status:%d", status);
            return status;
        }

        api_log_MsgDebug("����1:ǰ��:�ɹ�");
    #endif
    
    for (;;)
    {
        //���
        /*
        #ifdef SIGNLE_SEGMENT12345
            code = signlecopy1_pre_join2team(dmobj, ctxt);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("����1:���:����:����");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("����1:���:�ɹ�");
        #endif
        */
        
        api_log_MsgDebug("����1:����:��ʼ:tasknrref:%d", tasknrref);
        
        status = signle_pri_copy1(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����1:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����1:����:����:tasknrref:%d", tasknrref);
    }
    
    status = signlecopy1_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("����1:����:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����1:����:�ɹ�");

    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy2(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����2:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����2:����:����:tasknrref:%d", tasknrref);
    }
    
    #ifdef SIGNLE_SEGMENT12345_124
        status = signle_pri_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("����2:�س�:ʧ��:status:%d", status);
            return status;
        }

        api_log_MsgDebug("����2:�س�:�ɹ�");

        //�ȴ��ص�ƽ���ӹ�
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            dmobj->MoveTo(5, 5);
            Delay(200);
        
            //����Ƿ���롰ƽ���ӹȡ�
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("�س�:��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("�س�:��ͼ:ƽ���ӹ�:����:ʧ��");
        }
    #endif
#endif

#ifdef SIGNLE_SEGMENT3
    dmobj->KeyDownChar("=");
    Delay(300);
    dmobj->KeyUpChar("=");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("0");
    Delay(300);
    dmobj->KeyUpChar("0");
    Delay(300);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|451:97|450:107|450:115";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[450:115]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[450:115]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("��������:����:��Χ,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��������:����:ʧ��");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��������:����:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("��������:����:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        
        Delay(500);
    }

    //�ȴ���ͼ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "����������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:����������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:����������:����:ʧ��");
        Delay(1000);
    }

    //����11:��M�� 
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

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-vip.txt");
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

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    //994:114 -> 602:273
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 392, lposY + 159);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
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

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);

    pathofposxys = "139:236|138:248|137:257|125:257|114:257|105:257|96:257|88:257";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[88:257]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[88:257]:�ɹ�");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
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
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);

        //��ͼ�����������Ͷ���
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "�������", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("��ͼ:�������:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK������1" : "DK������2";
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, randtime, "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("%s:���:�ɹ�", randtime);
            continue;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        lposX = lposX2;
        lposY = lposY2;
        
        api_log_MsgDebug("%s:���:ʧ��, lposX2:%ld, lposY2:%ld", randtime, lposX2, lposY2);
        
        Delay(500);
    }

    //�ȴ��������������ͼ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "�������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:�������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:�������:����:ʧ��");
        Delay(1000);
    }

    //��������
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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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

    //Ѱ·
    pathofposxys = "87:43|98:44|109:45|120:46";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[120:46]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[120:46]:�ɹ�");

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("-");
    Delay(500);
    dmobj->KeyUpChar("-");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //TODO:��M:����13
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

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-vip.txt");
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

    //858:158 -> 388:402
    //for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX - 470, lposY + 244);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        /*dmobj->SetDict(0, FONTSIGNLE_DIR"font-posxy.txt");
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
    
    dmobj->KeyPressChar("f2");
    Delay(500);
    dmobj->KeyUpChar("f2");
    Delay(500);

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    //Ѱ·
    pathofposxys = "222:147|231:142|239:138|243:136";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[243:136]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[243:136]:�ɹ�");

    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy3(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����3:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����3:����:����:tasknrref:%d", tasknrref);
    }

    dmobj->MoveTo(5, 5);
    Delay(300);
    
    //����27
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("Ƶ��:����:ʧ��");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(1000);
    
    //���������Ϸ 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "����������", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:����������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:����������:����:ʧ��");
        Delay(500);
    }

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("����3->4:�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����3->4:�س�:�ɹ�");
    
    //�ȴ��ص�ƽ���ӹ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //����Ƿ���롰ƽ���ӹȡ�
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
    }
#endif

#ifdef SIGNLE_SEGMENT4
    #if defined(SIGNLE_SEGMENT12345_124) || defined(SIGNLE_SEGMENT45) || defined(SIGNLE_SEGMENT4)
        dmobj->KeyDownChar("=");
        Delay(300);
        dmobj->KeyUpChar("=");
        Delay(300);

        Delay_With_Terminate_Check(3, luaman_task, dmobj);
        
        dmobj->KeyDownChar("0");
        Delay(300);
        dmobj->KeyUpChar("0");
        Delay(300);

        Delay_With_Terminate_Check(3, luaman_task, dmobj);
        
        pathofposxys = "349:129|353:118|355:112|359:102|368:101|375:101|385:101|396:98|406:94|415:90|424:89|431:88|437:89|445:88|451:88|451:78|451:69|450:62|450:58|450:53";
        pathOfPosXYs = CString(pathofposxys);
        code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
        if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
        {
            api_log_MsgDebug("Ѱ·:ǰ��:����:[450:53]:����:����");
            return SNAIL_ERRNO_REALIVE;
        }

        api_log_MsgDebug("Ѱ·:ǰ��:����:[450:53]:�ɹ�");
    #endif

    //28��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
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
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[DAS]�ƶ���DK������", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("[DAS]�ƶ���DK������:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("[DAS]�ƶ���DK������:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        
        Delay(500);
    }
    
    //�����ͼ������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(100);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTSIGNLE_DIR"font-auxiend.txt");
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

    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->KeyPressChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);

    Delay_With_Terminate_Check(2, luaman_task, dmobj);
    
    pathofposxys = "419:409|428:409|433:401|437:395|441:388";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[441:388]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[441:388]:�ɹ�");
    
    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy4(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����4:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����4:����:����:tasknrref:%d", tasknrref);
    }

    //ֻ��124�������˲�ִ��
    #if defined(SIGNLE_SEGMENT124) || defined(SIGNLE_SEGMENT4)
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            code = check_alarm(dmobj, ctxt);
            if (ERRNO_ALARM == code)
            {
                Delay(1000);
                api_log_MsgDebug("׼��:������ɫ:����:���:�ɹ�");
                continue;
            }
            
            dmobj->KeyDownChar("esc");
            Delay(500);
            dmobj->KeyUpChar("esc");
            Delay(500);

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
            Delay(500);
        }
        
        for (;;)
        {
            dmobj->MoveTo(lposX, lposY);
            Delay(500);
            dmobj->LeftClick();
            Delay(500);
            dmobj->MoveTo(5, 5);
            Delay(500);

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
            code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                break;
            }

            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
            Delay(1000);
        }

        dmobj->MoveTo(5, 5);
        Delay(500);
            
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
            code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("Ƶ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                
                break;
            }

            dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
            code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "00ff00-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("Ƶ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                
                break;
            }
            
            api_log_MsgDebug("Ƶ��:����:ʧ��");
            Delay(500);
        }
        
        Delay_With_Terminate_Check(3, luaman_task, dmobj);

        //TODO:ѡ��ɫ����������Ϸ
        (void)luaman_config->enter_with_role(dmobj, ctxt);

        /*dmobj->MoveTo(640, 670);
        Delay(300);
        dmobj->LeftClick();
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(1000);*/
        
        //���������Ϸ 640:670
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
            Delay(500);
        }

        status = signle_pri_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgDebug("����3->4:�س�:ʧ��:status:%d", status);
            return status;
        }

        api_log_MsgDebug("����3->4:�س�:�ɹ�");
        
        //�ȴ��ص�ƽ���ӹ�
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->MoveTo(5, 5);
            Delay(200);
        
            //����Ƿ���롰ƽ���ӹȡ�
            Delay(1000);
            
            Check_Terminate_Thread(luaman_task, dmobj);
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                break;
            }

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
        }
    #endif
#endif

#ifdef SIGNLE_SEGMENT5
    //33��
    pathofposxys = "436:399|434:408|433:416|433:425|438:434";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[438:434]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[438:434]:�ɹ�");

    dmobj->KeyDownChar("0");
    Delay(500);
    dmobj->KeyUpChar("0");
    Delay(500);
    Delay(500);

    pathofposxys = "443:442|448:448|453:454|457:458|460:460";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[460:460]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[460:460]:�ɹ�");
    
    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy5(dmobj, ctxt, tasknr);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����5:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����5:����:����:tasknrref:%d", tasknrref);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////
    //ע�͵��һ�
    #ifdef HAVE_MAGIC_STONE
        int dianum = 20; //���صı�ʯ��n�Ի��Ĵ��� �ް�ƽ 2023/1/26 10:34:22
        for (i = 0; i < dianum; i++)
        {
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->KeyDownChar("n");
                Delay(500);
                dmobj->KeyUpChar("n");
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);
                
                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "���صı�ʯ", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("���صı�ʯ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                    break;
                }

                
                api_log_MsgDebug("���صı�ʯ:����:ʧ��");
                Delay(500);
            }

            int jix = 0;
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX, lposY);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "лл", "00ff00-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX2 = pposX2.lVal;
                    lposY2 = pposY2.lVal;

                    api_log_MsgDebug("лл:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
                    break;
                }

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "���صı�ʯ", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    if (jix == 4)
                    {
                        api_log_MsgDebug("���صı�ʯ:���:���");
                        goto Label_continue_talk;
                    }

                    api_log_MsgDebug("���صı�ʯ:���:����:%d", jix);

                    jix++;
                    continue;
                 
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                jix++;
                api_log_MsgDebug("���صı�ʯ:���:����, lposX:%ld, lposY:%ld, jix:%d", lposX, lposY, jix);
                Delay(500);
            }

            //��лл
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX2, lposY2);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict( 0, FONTSIGNLE_DIR"font-DuiHua10.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "лл", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("лл:����:ʧ��");
                    break;
                }

                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("лл:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            }
            
        Label_continue_talk:
            api_log_MsgDebug("�Ի�:����:%d", i);
        }
    #endif
    /////////////////////////////////////////////////////////////////////////////////////////////

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("Ƶ��:����:ʧ��");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(1000);
        
    //���������Ϸ 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
        Delay(500);
    }

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("����3->4:�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����3->4:�س�:�ɹ�");
    
    //�ȴ��ص�ƽ���ӹ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //����Ƿ���롰ƽ���ӹȡ�
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
    }
#endif

#ifdef SIGNLE_SEGMENT6
    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy6(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����6:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����6:����:����:tasknrref:%d", tasknrref);
    }

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("����6:�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����6:�س�:�ɹ�");

    //�ȴ��ص�ƽ���ӹ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //����Ƿ���롰ƽ���ӹȡ�
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�س�:��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�س�:��ͼ:ƽ���ӹ�:����:ʧ��");
    }
#endif

#ifdef SIGNLE_SEGMENT7
    tasknrref = 0;
    for (;;)
    {
        status = signle_pri_copy7(dmobj, ctxt, tasknrref);
        tasknrref++;

        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����7:����:���:����:��һ��:����:tasknrref:%d", tasknrref);
            break;
        }

        api_log_MsgDebug("����7:����:����:tasknrref:%d", tasknrref);
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ɫ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("esc:��Ϸ����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:����,ʧ��");
        Delay(500);
    }
    
    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-exitgame2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("esc:��Ϸ����:���,�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("esc:��Ϸ����:���,ʧ��");
        Delay(1000);
    }

    dmobj->MoveTo(5, 5);
    Delay(500);
        
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-channelx.txt");
        code = dmobj->FindStrFast(1040, 110, 1270, 240, "Ƶ��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Ƶ��:��ɫ:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            
            break;
        }
        
        api_log_MsgDebug("Ƶ��:����:ʧ��");
        Delay(500);
    }
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);

    dmobj->MoveTo(640, 670);
    Delay(300);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
        
    //���������Ϸ 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-DiTUMingZhi1.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "����ɽ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:����ɽ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:����ɽ��:����:ʧ��");
        Delay(500);
    }
    
    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("����7:�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("����7:�س�:�ɹ�");

    //�ȴ��ص�ƽ���ӹ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //����Ƿ���롰ƽ���ӹȡ�
        Delay(1000);
        
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-map.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
    }
    
#endif
    
    return SNAIL_ERRNO_SUCCESS;
}

static void __this_null(void){}
