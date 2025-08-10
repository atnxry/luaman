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
#include "taskEx175.h"

#define CONFIG_EX175_XML  "conf\\ex175.xml"
#define FONTEX175_DIR "font\\ex175\\"
#define BMPEX175_DIR "bmp\\ex175\\"

static struct luaman_posxy deltaatpos175[] = 
{
    {0,-15}, //4 ��N�������ɫ��ʥħ֮צ.(���볡���ŵ����λ�ã���Ϊ��������ɫ��ʥħ֮צ��
};

//���������Ƿ�����,��ʱʱ��(��λ:��),�Ƿ��λ,��ֽ�������,�ֿ�,��ɫƫ��ֵ
//���ֽ���:
//  {FONT_FIND, 20, ADJUST_POS, {"���ѿ���",     FONT_EXPIRATE_DIR"font-xxx.txt", "ffffff-101010"}}
//�����������������,���4~����ֶβ�����д,��:
//  {NO_FONT_FIND, 20, ADJUST_POS}
//һ����7��
static struct font_info_of_fight_t fif175_info[] =
{
    {FONT_FIND,    30, ADJUST_POS,   {"������о�����",     FONTEX175_DIR"font-175tishi-1.txt", "ffffff-101010"}}, //��֣�1��
    {FONT_FIND,    120, ADJUST_POS,   {"�ҵ�����",     FONTEX175_DIR"font-175tishi-2.txt", "00ff00-101010"}}, //���2
    {NO_FONT_FIND, 25, ADJUST_POS,  },
    //{NO_FONT_FIND, 5, ADJUST_POS,  },
    {NO_FONT_FIND, 25, ADJUST_POS,  },
    {NO_FONT_FIND, 20, ADJUST_POS,  },
    {FONT_FIND,    220, ADJUST_POS,   {"�ҿ�����",     FONTEX175_DIR"font-175tishi-3.txt", "00ff00-101010"}},
};

//Ѱ·����,��12��Ѱ·����
static char* walk175_path[] =
{
    "466:239|461:239|456:239", //Ѱ·��1��
    "450:239|444:238|437:237|432:237|426:237|420:238|415:238|411:238",
    "405:239|401:244|398:249|392:252|386:250|383:245|379:239|370:239|361:239|352:239|343:239|334:239|327:239|321:239",
    "315:239|309:239|302:243|296:245|289:244|283:240|275:237",
    //TODO:�ٵ�ǰ�е������һ��
    "271:237|264:237|257:237|250:237|244:237|238:238",
    "229:237|223:233",
    "223:233|218:225|214:218|213:212|213:205|212:198|212:191|212:184|212:175|212:168|213:162|213:158",
    "213:150|214:141|221:138|223:133",
    "216:137|215:146|213:153|213:159|213:167|213:176|213:184|213:193|213:201|213:208|213:215|213:222|213:231|213:242|213:250|212:257|212:264|212:272|212:279|212:284|212:290|213:296",
    "213:300|215:307|216:325|223:326",
    "215:316|213:309|213:302|213:295|213:287|213:279|213:272|212:264|212:257|209:252|205:245|201:239|195:236",
    "191:239|184:236|177:235|169:236|160:236|154:235",
    "142:237|136:236|129:239|125:241|118:238|112:236|106:234|100:234|95:235|88:235|82:235|75:234|69:234|63:234|57:234",
};

struct deltaval deltaatpos175_u[] =
{
    {24, 82}, //7 Ѱ·7���ҵ���ɫ�� ������ݸˡ����������ݸ˵����λ��1.
    {24, 82}, //9 Ѱ·9���ҵ���ɫ�� ������ݸˡ����������ݸ˵����λ��2.
};


static int ex175Checkkeystate(Idmsoft* dmobj, void* ctxt)
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

static int ex175Checklife(Idmsoft* dmobj, void* ctxt, int recusive)
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

    dmobj->SetDict(0, FONTEX175_DIR"font-blood.txt");
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

        //api_log_MsgDebug("Ѫ��:����,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
        //    hwnd, bBloodAva, bBloodTot);
    }
    else
    {
        strBBloodLen = strBBlood.GetLength();
        bBloodAva = atol(strBBlood.Mid(1, strBBloodLen - maxBlueFiled));
        bBloodTot = atol(strBBlood.Mid(strBBloodLen - maxBlueFiled + 1, strBBloodLen));

        //api_log_MsgDebug("Ѫ��:����:�ɹ�,hwnd:%ld, bBloodAva:%ld, bBloodTot:%ld",
        //    hwnd, bBloodAva, bBloodTot);
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

    ex175Checkkeystate(dmobj, ctxt);

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

        dmobj->SetDict(0, FONTEX175_DIR"font-chcklife.txt");
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

        dmobj->SetDict(0, FONTEX175_DIR"font-chcklife.txt");
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
        code = ex175Checklife(dmobj, ctxt, 1);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("����:վ��:���");
            break;
        }

        Delay_With_Terminate_Check(1, luaman_task, dmobj);
    }

    code = common_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != code)
    {
        api_log_MsgError("����:ʧ��:code:%d", code);
    }
    
    return SNAIL_ERRNO_REALIVE;
}


static int ex175Walk(Idmsoft* dmobj, void* ctxt, CString& tgtPosXYs, int cdir)
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
                (void)ex175Checkkeystate(dmobj, ctxt);

                api_log_MsgDebug("Ѱ·:����:��ס,ֹͣѰ·:�ȴ�������ʧ,��һ����, tposX:%ld, tposY:%ld",
                    tposX, tposY);
                
                //�ȴ��ض�������ʧ
                goto LABEL_WALK_BLOCK_EXIT_CHECK;
            }

            //�ȼ����Ѫ��
            code = ex175Checklife(dmobj, ctxt, 0);
            if (SNAIL_ERRNO_REALIVE == code)
            {
                api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
                return code;
            }

            //Check_Terminate_Thread(luaman_task, dmobj);
            
            //��ȡ��ǰ����
            dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
            strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
            dmobj->ClearDict(0);
            if (CString("") == strPosXY)
            {
                (void)ex175Checkkeystate(dmobj, ctxt);
                
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

                (void)ex175Checkkeystate(dmobj, ctxt);
                
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
        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:��ס:Ѫ��:���:����:����");
            return code;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-map.txt");
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

static int ex175FightTimeout(Idmsoft* dmobj, void* ctxt, int withu, struct font_info_of_fight_t* fif)
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
    time_t lastAdjposTime = time(NULL);
    CString pathOfPosXYs;
    int findfont = fif->findfont;
    int timeout = fif->timeout;
    int adjust_pos = fif->adjust_pos;
    struct font_info_t* fontinfo = &fif->font;
    
    api_log_MsgDebug("���:��ʼ, adjust_pos:%d", adjust_pos);

    if (adjust_pos)
    {
        code = GetCurPosXY(dmobj, ctxt, NULL, NULL, &pathOfPosXYs);
        if (ERRNO_SUCCESS != code)
        {
            adjust_pos = 0;
            api_log_MsgDebug("GetCurPosXY failed, adjust_pos:%d, code:%d", adjust_pos, code);
        }
    }

    lastAdjposTime = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
            return code;
        }
        
        if (withu)
        {
            dmobj->KeyDownChar("u");
            Delay(50);
            dmobj->KeyUpChar("u");
            Delay(50);
        }
        
        for (idx = 0; idx < 8; ++idx)
        {
            dmobj->KeyDown(48 + idx + 1);
            Delay(50);
            dmobj->KeyUp(48 + idx + 1);
            Delay(50);
        }

        if (findfont)
        {
            dmobj->SetDict(0, fontinfo->font);
            code = dmobj->FindStrFast(0, 27, 1279, 826, fontinfo->word, fontinfo->color_offset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("%s:����:�ɹ�, lposX:%ld, lposY:%ld", fontinfo->word, lposX, lposY);
                break;
            }
        }

        if (timeout > 0)
        {
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (timeout * 1.0))
            {
                api_log_MsgDebug("����:��ʱ:%d��", timeout);
                break;
            }

            api_log_MsgDebug("����:û��:��ʱ:%d��:%d��", timeout, deltaTime);
        }

        if (adjust_pos)
        {
            deltaTime = difftime(time(NULL), lastAdjposTime);
            if (deltaTime < 15.0)
            {
                continue;
            }

            api_log_MsgDebug("���:��λ");
            
            code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
            if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
            {
                api_log_MsgDebug("���:��λ:����:����:����");
                return SNAIL_ERRNO_REALIVE;
            }

            api_log_MsgDebug("���:��λ:����:�ɹ�");

            lastAdjposTime = time(NULL); //���¹�λʱ��
        }
        api_log_MsgDebug("����:ʧ��");
        Delay(500);
    }

    for (idx = 0; idx < 80; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightAutoStop(Idmsoft* dmobj, void* ctxt)
{
    dmobj->KeyDownChar("w");
    Delay(25);
    dmobj->KeyUpChar("w");
    Delay(25);
    
    dmobj->KeyDownChar("s");
    Delay(25);
    dmobj->KeyUpChar("s");
    Delay(25);

    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightAutoStart(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    int code = 0;
    VARIANT pposX, pposY;
    long lposX = 0, lposY = 0;
    VARIANT pposX2, pposY2;
    long lposX2 = 0, lposY2 = 0;
    
    for (;;)
    {
        dmobj->MoveTo(105, 143);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(300);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(300);

        dmobj->SetDict(0, FONTEX175_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ս��:����:��:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("ս��:����:��:ʧ��");
        LUAMAN_WAIT_MS(500);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ս��:����:��:����:����");
            return code;
        }
    }

    for (;;)
    {
        dmobj->MoveTo(lposX, lposY);
        LUAMAN_WAIT_MS(300);
        dmobj->LeftClick();
        LUAMAN_WAIT_MS(500);
        dmobj->MoveTo(5, 5);
        LUAMAN_WAIT_MS(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-fightauto.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʼ", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("ս��:����:��ʼ:ʧ��,lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        LUAMAN_WAIT_MS(500);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("ս��:����:��ʼ:����:����");
            return code;
        }
    }

    return SNAIL_ERRNO_SUCCESS;
}

static int ex175FightTimeoutAuto(Idmsoft* dmobj, void* ctxt, struct font_info_of_fight_t* fif)
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
    CString pathOfPosXYs;
    int findfont = fif->findfont;
    int timeout = fif->timeout;
    int adjust_pos = fif->adjust_pos;
    struct font_info_t* fontinfo = &fif->font;
    
    api_log_MsgDebug("�Զ�:���:��ʼ");

    code = ex175FightAutoStart(dmobj, ctxt);
    if (SNAIL_ERRNO_REALIVE == code)
    {
        api_log_MsgDebug("�Զ�:���:��ʼ:Ѫ��:����,����");
        return code;
    }

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);

        code = ex175Checklife(dmobj, ctxt, 0);
        if (SNAIL_ERRNO_REALIVE == code)
        {
            api_log_MsgDebug("Ѱ·:Ѫ��:���,����������");
            return code;
        }
        
        if (findfont)
        {
            dmobj->SetDict(0, fontinfo->font);
            code = dmobj->FindStrFast(0, 27, 1279, 826, fontinfo->word, fontinfo->color_offset, 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("%s:����:�ɹ�, lposX:%ld, lposY:%ld", fontinfo->word, lposX, lposY);
                break;
            }
        }

        if (timeout > 0)
        {
            deltaTime = difftime(time(NULL), lastChckTime);
            if (deltaTime > (timeout * 1.0))
            {
                api_log_MsgDebug("����:��ʱ:%d��", timeout);
                break;
            }

            api_log_MsgDebug("����:û��:��ʱ:%d��:%d��", timeout, deltaTime);
        }

        api_log_MsgDebug("����:ʧ��");
        Delay(500);
    }

    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    ex175FightAutoStop(dmobj, ctxt);
    
    for (idx = 0; idx < 70; idx++)
    {
        dmobj->KeyDownChar("space");
        Delay(20);
        dmobj->KeyUpChar("space");
        Delay(20);
    }

    api_log_MsgDebug("�Զ�:���:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int __doworkex175(Idmsoft* dmobj, void* ctxt, int dotask_num)
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
    VARIANT pposX5, pposY5;
    VARIANT pposX7, pposY7;
    VARIANT pposX8, pposY8;
    long posX = 0, posY = 0;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX5 = 0, lposY5 = 0;
    long lposX7 = 0, lposY7 = 0;
    long lposX8 = 0, lposY8 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    int j = 0;
    char* path = NULL;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int deltaX = 0;
    int deltaY = 0;
    char* randtime = NULL;
    int status = 0;
    int auto_walk_index = 0;
    int auto_fight_index = 0;

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
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-auxiend.txt");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-auxiend.txt");
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
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�˳�����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            dmobj->SetDict( 0, FONTEX175_DIR"font-auxiend.txt");
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
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-cpy6_npc5.txt");
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

        dmobj->SetDict(0, FONTEX175_DIR"font-cpy6_npc5.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "�볡����", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�볡����:���:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        dmobj->SetDict(0, FONTEX175_DIR"font-npc100002.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX2, &pposY2);
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

                dmobj->SetDict(0, FONTEX175_DIR"font-npc100002.txt");
                code = dmobj->FindStrFast(390, 27, 1279, 826, "��������", "00ff00-101010", 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("��������:���:�ɹ�");
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

        dmobj->SetDict(0, FONTEX175_DIR"font-cpy6_npc5.txt");
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
        
        dmobj->KeyDownChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict( 0, FONTEX175_DIR"font-cpy6_npc5.txt");
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

    deltaX = deltaatpos175[0].x;
    deltaY = deltaatpos175[0].y;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX + deltaX, lposY + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-okcancel.txt");
        code = dmobj->FindStrFast(390, 27, 1279, 826, "ȷ��", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("ȷ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        code = common_checkcopyfull(dmobj, ctxt);
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

        code = common_checkcopyfull(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("ȷ��:���:������:���:����");
            goto Label_NPC_next_retry;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-okcancel.txt");
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

    dmobj->KeyPressChar("=");
    Delay(500);
    dmobj->KeyUpChar("=");
    Delay(500);

    Delay_With_Terminate_Check_Alram(3, luaman_task, dmobj);
    
    dmobj->KeyDownChar("shift");
    Delay(200);
    dmobj->KeyDownChar("e");
    Delay(200);
    dmobj->KeyUpChar("shift");
    Delay(200);
    dmobj->KeyUpChar("e");
    Delay(200);
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���1:�ɹ�");
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���2:�ɹ�");
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���3:�ɹ�");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    /*
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���4:�ɹ�");
    */
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);
    
    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("7");
        Delay(500);
        dmobj->KeyUpChar("7");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    
    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���5:�ɹ�");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    dmobj->KeyDownChar("a");
    Delay(1000);
    dmobj->KeyUpChar("a");
    Delay(500);

    //�һ�ɫ��������ݸ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("������ݸ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("������ݸ�:����:ʧ��");
        Delay(500);
    }

    deltaX = deltaatpos175_u[0].deltax;
    deltaY = deltaatpos175_u[0].deltay;
    
    //�����ɫ��������ݸ�
    int idx = 0;
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX5 + deltaX, lposY5 + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("������ݸ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("������ݸ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("~");
    Delay(500);
    dmobj->KeyUpChar("~");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    dmobj->KeyDownChar("s");
    Delay(1000);
    dmobj->KeyUpChar("s");
    Delay(500);
    
    //�һ�ɫ��������ݸ�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX5, &pposY5);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX5 = pposX5.lVal;
            lposY5 = pposY5.lVal;

            api_log_MsgDebug("������ݸ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX5, lposY5);
            break;
        }

        api_log_MsgDebug("������ݸ�:����:ʧ��");
        Delay(500);
    }

    deltaX = deltaatpos175_u[1].deltax;
    deltaY = deltaatpos175_u[1].deltay;
    
    //�����ɫ��������ݸ�
    for (idx = 0; idx < 1; idx++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX5 + deltaX, lposY5 + deltaY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "������ݸ�", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("������ݸ�:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("������ݸ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("u");
        Delay(500);
        dmobj->KeyUpChar("u");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("npc1:�Ի�:����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("npc1:�Ի�:����:����:ʧ��");
        Delay(500);
    }

    dmobj->KeyDownChar("5");
    Delay(500);
    dmobj->KeyUpChar("5");
    Delay(500);

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    //����ɫ���������
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
        
        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "00ff00-101010", 0.9, &pposX7, &pposY7);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX7 = pposX7.lVal;
            lposY7 = pposY7.lVal;

            api_log_MsgDebug("�Ի�:���������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX7, lposY7);
            break;
        }

        api_log_MsgDebug("�Ի�:���������:����:ʧ��");
        Delay(500);
    }

    //�����ɫ���������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX7, lposY7);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���������", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("���������:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
    
        api_log_MsgDebug("���������:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���6:�ɹ�");

    //�ȴ������� 142:237
    for (;;)
    {
        dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                //bug:���̫����ܾ���Ŀ��㸽��,ʵ�ʲ�û�з�£ 2022/01/08 15:43:16
                if (abs(posX - 142) <= 1 && abs(posY - 237) <= 1)
                {
                    api_log_MsgDebug("����:142:237:ƥ��");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("����:142:237:δƥ��");
        Delay(1000);
    }
    
    //code = ex175FightTimeout(dmobj, ctxt, 1, &fif175_info[auto_fight_index++]);
    code = ex175FightTimeoutAuto(dmobj, ctxt, &fif175_info[auto_fight_index++]);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("���:��ʱ:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("���7:�ɹ�");

    pathofposxys = walk175_path[auto_walk_index++];
    pathOfPosXYs = CString(pathofposxys);
    code = ex175Walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:%s:����:����", pathofposxys);
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:%s:�ɹ�", pathofposxys);

    dmobj->KeyDownChar("9");
    Delay(500);
    dmobj->KeyUpChar("9");
    Delay(500);
    
    //����ɫ���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���븱��", "00ff00-101010", 0.9, &pposX8, &pposY8);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX8 = pposX8.lVal;
            lposY8 = pposY8.lVal;

            api_log_MsgDebug("�Ի�:���븱��:����:�ɹ�, lposX8:%ld, lposY8:%ld", lposX8, lposY8);
            break;
        }

        api_log_MsgDebug("�Ի�:���븱��:����:ʧ��");
        Delay(500);
    }

    //�����ɫ���븱��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX8, lposY8);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTEX175_DIR"font-175DuiHua4.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���븱��", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�Ի�:���븱��:���:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("�Ի�:���븱��:����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(100);

    status = common_exitgroup(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("����:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("�˶�:�ɹ�");
    return 0;
}

static int _doworkex175(Idmsoft* dmobj, void* ctxt, int dotask_num)
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
    VARIANT pposX5, pposY5;
    VARIANT pposX7, pposY7;
    VARIANT pposX8, pposY8;
    long posX = 0, posY = 0;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX5 = 0, lposY5 = 0;
    long lposX7 = 0, lposY7 = 0;
    long lposX8 = 0, lposY8 = 0;
    long lposX0 = 0, lposY0 = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    int j = 0;
    char* path = NULL;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int deltaX = 0;
    int deltaY = 0;
    char* randtime = NULL;
    int status = 0;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->SetDict(0, FONTEX175_DIR"font-175MingZhi6.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:������˹:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:������˹:����:ʧ��");
        Delay(1000);
    }

    code = common_exitgroup(dmobj, ctxt);
    if (ERRNO_SUCCESS != code)
    {
        return code;
    }
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTEX175_DIR"font-talk3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ���DK������", "00ff00-101010", 0.9, &pposX, &pposY);
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
        dmobj->SetDict(0, FONTEX175_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }
        
        randtime = ((time(NULL)) % 2)? "DK������1" : "DK������2";
        
        dmobj->SetDict(0, FONTEX175_DIR"font-talk3.txt");
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

    //�ȴ������ͼ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONTEX175_DIR"font-maptips.txt");
        code = dmobj->FindStrFast(1140, 29, 1273, 58, "������˹Ұ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("��ͼ:������˹Ұ��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ͼ:������˹Ұ��:����:ʧ��");
        Delay(1000);
    }

    //��M���ҹ����������λ��2
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

        dmobj->SetDict(0, FONTEX175_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ʥħ֮צ", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ʥħ֮צ:����:�ɹ�,lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("ʥħ֮צ:����:ʧ��");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:�ƶ�:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        api_log_MsgDebug("����:�ƶ�:ʧ��");
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
    
        dmobj->SetDict(0, FONTEX175_DIR"font-npc2.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ƶ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("�ƶ�:���:�ɹ�");
            
            break;
        }

        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("���ƶ�:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    //�ȴ������� 16:12
    for (;;)
    {
        dmobj->SetDict(0, FONTEX175_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                //bug:���̫����ܾ���Ŀ��㸽��,ʵ�ʲ�û�з�£ 2022/01/08 15:43:16
                if (abs(posX - 16) <= 1 && abs(posY - 12) <= 1)
                {
                    api_log_MsgDebug("����:16:12:ƥ��");
                    break;
                }
            }
        }

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        api_log_MsgDebug("����:16:12:δƥ��");
        Delay(1000);
    }

    for (;;)
    {
        status = __doworkex175(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_NOMORE == status)
        {
            api_log_MsgDebug("����:ex175:ѭ��:����,dotask_num:%d", dotask_num);
            break;
        }

        api_log_MsgDebug("����:ex175:ѭ��:����,dotask_num:%d", dotask_num);
    }
    
    return 0;
}


static int doworkex175(struct cluamanaccountinfo* accountinfo, int roleno, struct common_loop_ctxt_t* clctxt)
{
    Idmsoft* dmobj = clctxt->dmobj;
    void* ctxt = clctxt->ctxt;
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
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dotask_num = 0;
    
    //ִ�С����񸱱���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = _doworkex175(dmobj, ctxt, dotask_num);
        if (SNAIL_ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("��ɫ:����:ִ��:���,dotask_num:%d, code:%d",
                dotask_num + 1, code);
            break;
        }

        dotask_num++;
        api_log_MsgDebug("����:����:����,dotask_num:%d", dotask_num);

        if (1 == dotask_num)
        {
            api_log_MsgDebug("����:����:���:����:��һ��:�˻�");
            break;
        }
    }

    api_log_MsgDebug("����:����:���,dotask_num:%d", dotask_num);
    
    return 0;
}

int api_TaskEx175Init(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    int curBindHostIdx = luaman_config->curBindHostIdx;
    
    api_log_MsgDebug("����:Ex175:��ʼ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);
    api_log_MsgDebug("curBindHostIdx  : %d", curBindHostIdx);
    
    api_PrintConfig(luaman_config);
    
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("����:��:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:��:�ɹ�, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("����:���:����:ʧ��, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:���:����:�ɹ�, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskEx175Main(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int bindWnd = luaman_config->curBindWnd;
    struct cluamanbindinfo* bpos = NULL, *bn = NULL, *bnode = NULL;
    
    api_log_MsgDebug("����:Ex175:��ѭ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        if (bindWnd != bpos->bindWndnr)
        {
            api_log_MsgDebug("ExSignleMain bindwnd unexpected, bindWnd:%ld. bindWndnr:%ld",
                bindWnd, bpos->bindWndnr);
            continue;
        }

        //return mainloop(&bpos->accountList, dmobj, ctxt);
        struct common_loop_ctxt_t clctxt;
        LUAMAN_MEMSET(&clctxt, 0, sizeof(struct common_loop_ctxt_t));
        clctxt.accountList = &bpos->accountList;
        clctxt.dmobj = dmobj;
        clctxt.ctxt = ctxt;
        clctxt.fontdir = FONTEX175_DIR;
        clctxt.bmpdir = BMPEX175_DIR;
        clctxt.configXML = CONFIG_EX175_XML;
        clctxt.pre_work = common_prework;
        clctxt.post_work = common_postwork;
        clctxt.do_work = doworkex175;
        
        return common_mainloop(&clctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskEx175Exit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("����:Ex175:�˳�");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}


