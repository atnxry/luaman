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
#include "taskSignin.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"

#define FONTSIGNIN_DIR "font\\Signin\\"
#define BMPSIGNIN_DIR "bmp\\Signin\\"


int api_TaskSigninInit(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("����:Signin:��ʼ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

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
    
    return ERRNO_SUCCESS;
}

static int reademail(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int code = 0;
    int status = 0;
    VARIANT pposX, pposY;
    VARIANT pposX2, pposY2;
    long lposX = 0, lposY = 0;
    long lposX2 = 0, lposY2 = 0;
    long lposX0 = 0, lposY0 = 0;
    int num = 5;
    int i = 0;

    //��n�Ի�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyPressChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNIN_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[�ż�]��ʹ�����䡣", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("[�ż�]��ʹ�����䡣:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("[�ż�]��ʹ�����䡣:����:ʧ��");
        Delay(500);
    }

    //�����[�ż�]��ʹ�����䡣��
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNIN_DIR"font-npc1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "[�ż�]��ʹ�����䡣", "00ff00-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("[�ż�]��ʹ�����䡣:���:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;
            
        api_log_MsgDebug("[�ż�]��ʹ�����䡣:���:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (i = 0; i < num; i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-email.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "ӣ������ƽ�ʱ�佱��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("ӣ������ƽ�ʱ�佱��:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            goto next;
        }

        //api_log_MsgDebug("ӣ������ƽ�ʱ�佱��:����:ʧ��");
        Delay(500);
    }

    status = ERRNO_NOT_EXIST;
    goto finish;
    
next:
    //����Ķ��ʼ�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(100);

        dmobj->SetDict(0, FONTSIGNIN_DIR"font-reademail.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ʼ�Ԥ��", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("�ʼ�Ԥ��:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ʼ�Ԥ��:����:ʧ��");
        Delay(500);
    }

    //�����������Ʒ�� 642:121 -> 682:627
    dmobj->MoveTo(lposX2 + 40, lposY2 + 506);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    //�����ɾ���� 642:121 -> 660:705
    dmobj->MoveTo(lposX2 + 18, lposY2 + 584);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    status = ERRNO_SUCCESS;
    
finish:
    //��esc�˳�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->KeyDownChar("esc");
        Delay(500);
        dmobj->KeyUpChar("esc");
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(500);
        
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-emailrbox.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ռ���", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����:�ر�:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����:�ر�:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return status;
}

static int signinwork(struct cluamanaccountinfo* accountinfo, Idmsoft* dmobj, void* ctxt)
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
    int i = 0;
    struct passwd2{char ch;char* path;} bmps[]=
    {
        {'0', BMPSIGNIN_DIR"0.bmp"},
        {'1', BMPSIGNIN_DIR"1.bmp"},
        {'2', BMPSIGNIN_DIR"2.bmp"},
        {'3', BMPSIGNIN_DIR"3.bmp"},
        {'4', BMPSIGNIN_DIR"4.bmp"},
        {'5', BMPSIGNIN_DIR"5.bmp"},
        {'6', BMPSIGNIN_DIR"6.bmp"},
        {'7', BMPSIGNIN_DIR"7.bmp"},
        {'8', BMPSIGNIN_DIR"8.bmp"},
        {'9', BMPSIGNIN_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int email_num = 0;
    time_t waitEmailStart;
    time_t waitEmailEnd;
    double timeDelta = 0.0;
    
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);

label_login:
    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //���ҡ��ʺš�
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ʺ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("�ʺ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("�ʺ�:����:ʧ��");
        Delay(500);
    }

    //�ʺ� 490:648 -> 600:655
    //dmobj->MoveTo(603, 647);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 7);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->user_name);
    if (1 != code)
    {
        api_log_MsgError("�ʺ�:%s:����:ʧ��", accountinfo->user_name);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("�ʺ�:%s:����:�ɹ�", accountinfo->user_name);

    //���� 490:648 -> 600:680
    //dmobj->MoveTo(603, 672);
    dmobj->MoveTo(lposX2 + 110, lposY2 + 32);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    code = dmobj->SendString(hwnd, accountinfo->passwd);
    if (1 != code)
    {
        api_log_MsgError("����:%s:����:ʧ��", accountinfo->passwd);
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("����:%s:����:�ɹ�", accountinfo->passwd);

    //������Ϸ 490:648 -> 530:720
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //dmobj->MoveTo(509, 730);
        dmobj->MoveTo(lposX2 + 50, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ʺ�", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (0 != code)
        {
            api_log_MsgDebug("�˻�:%s:��¼:�ɹ�", accountinfo->user_name);
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("�˻�:%s:��¼:ʧ��, lposX:%ld, pposY:%ld", accountinfo->user_name, lposX, pposY);
        Delay(1000);
    }

    //����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������������", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            lposX0 = lposX;
            lposY0 = lposY;
            
            api_log_MsgDebug("����:����:����:�ɹ�, lposX:%ld, pposY:%ld", lposX, lposY);
            break;
        }

        //����Ƿ���ִ���
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-loginerr.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�������������µ�¼��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�������������µ�¼��:����:�ɹ�, lposX:%ld, pposY:%ld", lposX, lposY);

            //������������������µ�¼���� 523:242 -> 640:533
            for (;;)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                
                dmobj->MoveTo(lposX + 117, lposY + 291);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONTSIGNIN_DIR"font-loginerr.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "�������������µ�¼��", "ffffff-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (-1 == code)
                {
                    api_log_MsgDebug("�������������µ�¼��:���:�ɹ�");
                    goto label_login;
                }

                lposX2 = pposX2.lVal;
                lposY2 = pposY2.lVal;

                api_log_MsgDebug("�������������µ�¼��:���:ʧ��, lposX2:%ld, pposY2:%ld", lposX2, lposY2);
            }
        }

        api_log_MsgDebug("����:����:����:ʧ��");
        Delay(1000);
    }

    for (i = 0; i < strlen(accountinfo->passwdex); i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        path = NULL;
        for (j = 0; j < sizeof(bmps)/sizeof(bmps[0]); j++)
        {
            if (bmps[j].ch == accountinfo->passwdex[i])
            {
                path = bmps[j].path;
                break;
            }
        }

        if (NULL == path)
        {
            api_log_MsgError("��������:������:��֧��");
            return ERRNO_NOT_SUPPORT;
        }
        
        api_log_MsgDebug("����:����:����:%s", path);

        dmobj->MoveTo(5, 5);
        Delay(200);
        
        for (;;)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONTSIGNIN_DIR"font-security.txt");
            code = dmobj->FindPic(496, 310, 781, 417, path, "101010", 0.9, 0, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;
            
                api_log_MsgDebug("����:����:%s:����:�ɹ�, lposX:%ld, lposY:%ld", path, lposX, lposY);
                break;
            }

            api_log_MsgDebug("����:����:%s:����:ʧ��", path);
            Delay(1000);
        }

        //�����������
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);

        api_log_MsgDebug("���:����:����:%s:�ɹ�", path);
    }

    //���ȷ��,����������������: 586:266 -> 686:430
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX0 + 100, lposY0 + 164);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������������", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����:����:ȷ��:�ɹ�");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("����:����:ȷ��:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("����:��Ϸ:�ɹ�");

    //�����������Ϸ�� 640:670
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(640, 670);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        //����Ƿ���롰ƽ���ӹȡ�
        Delay(1000);
        
        for (i = 0; i < 5; i++)
        {
            Check_Terminate_Thread(luaman_task, dmobj);
            
            dmobj->SetDict(0, FONTSIGNIN_DIR"font-map.txt");
            code = dmobj->FindStrFast(1140, 29, 1273, 58, "ƽ���ӹ�", "ffffff-101010", 0.9, &pposX, &pposY);
            dmobj->ClearDict(0);
            if (-1 != code)
            {
                lposX = pposX.lVal;
                lposY = pposY.lVal;

                api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
                goto next;
            }

            api_log_MsgDebug("��ͼ:ƽ���ӹ�:����:ʧ��");
            Delay(1000);
        }
    }

next:
    //���ҡ�ӣ������ƽ�ʱ�佱����
    waitEmailStart = time(NULL);
    
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        code = reademail(dmobj, ctxt);
        if (ERRNO_SUCCESS != code)
        {
            if (0 == luaman_config->waitEmail || email_num > 0)
            {
                break;
            }

            waitEmailEnd = time(NULL);
            timeDelta = difftime(waitEmailEnd, waitEmailStart);
            if (timeDelta > 20 * 60)
            {
                api_log_MsgInfo("�ʼ�:%s:�Ķ�:�ȴ�:��ʱ", accountinfo->user_name);
                break;
            }
            
            //10���Ӻ�����
            for (i = 0; i < 10; i++)
            {
                Check_Terminate_Thread(luaman_task, dmobj);
                Delay(1000);
            }
            continue;
        }

        email_num++;
    }

    api_log_MsgDebug("�ʼ�:�Ķ�:���");

    //�˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //��� 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNIN_DIR"font-logout.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "���µ�¼", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("���µ�¼:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("���µ�¼:����:ʧ��");
        Delay(500);
    }

    //���"���µ�¼"
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONTSIGNIN_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("��Ϸ��Ϣ����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("��Ϸ��Ϣ����:����:ʧ��");
        Delay(500);
    }

    //�ȴ�����Ϸ��Ϣ���桱��ʧ
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        dmobj->SetDict(0, FONTSIGNIN_DIR"font-saveconf.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��Ϸ��Ϣ����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("��Ϸ��Ϣ����:���");
            break;
        }

        lposX = pposX.lVal;
        lposY = pposY.lVal;
            
        api_log_MsgDebug("��Ϸ��Ϣ����:����, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return ERRNO_SUCCESS;
}

static int TaskSigninMainLoop(struct list_head* accountList, Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int status = 0;
    int bindWnd = luaman_config->curBindWnd;
    struct cluamanaccountinfo* apos = NULL, *an = NULL, *anode = NULL;

    list_for_each_entry_safe(apos, an, accountList, node, struct cluamanaccountinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        status = signinwork(apos, dmobj, ctxt);
        if (ERRNO_SUCCESS != status)
        {
            api_log_MsgError("ǩ��:%s:ʧ��, status:%d", apos->user_name, status);
        }
    }
    
    return ERRNO_SUCCESS;
}

int api_TaskSigninMain(Idmsoft* dmobj, void* ctxt)
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
    
    api_log_MsgDebug("����:Signin:��ѭ��");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    list_for_each_entry_safe(bpos, bn, &luaman_config->bindinfoList, node, struct cluamanbindinfo)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        //if (bindWnd + 1 != bpos->bindWndnr)
        if (bindWnd != bpos->bindWndnr)
        {
            continue;
        }

        return TaskSigninMainLoop(&bpos->accountList, dmobj, ctxt);
    }

    return ERRNO_SUCCESS;
}

int api_TaskSigninExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    int bindWnd = luaman_config->curBindWnd;
    
    api_log_MsgDebug("����:Signin:�˳�");
    api_log_MsgDebug("hwnd            : %ld", hwnd);
    api_log_MsgDebug("groupid         : %d", groupid);
    api_log_MsgDebug("role            : %s", roletips[role]);
    api_log_MsgDebug("bindWnd         : %ld", bindWnd);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
        
    return ERRNO_SUCCESS;
}

