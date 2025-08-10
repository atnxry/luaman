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
#include "taskTeamGroup.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"

#define FONT_TEAMGROUP_DIR "font\\teamgroup\\"
#define BMP_TEAMGROUP_DIR "bmp\\teamgroup\\"

struct rect_t
{
    posxy_t lt;
    posxy_t rb;
};

struct teammate_t
{
    char* word;
    char* color_offset;
};

struct teammate_info_t
{
    char* font;
    struct teammate_t names[16];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���λ��
static posxy_t __ti_offset[] =
{
    {30, 60},
    {104, 7},
    {206, 8},
};

//���ҷ�Χ
static rect_t __lookup_rect = 
{
    {480, 27},  //���Ͻ�����
    {920, 170}, //���½�����
};
    
static struct teammate_info_t __ti_info[] =
{
    //�ֿ�1
    {
        FONT_TEAMGROUP_DIR"font-libraray1.txt",
        {
            //-----------------------  ��ʼ  ----------------------------------------
            //�ֿ�1��Ҫ�ҵ��ּ�����ɫ,���16��Ҫ�ҵ���,�������������
            {"�Թⷴ��˼", "ffffff-101010"},
            {"���������", "ffffff-101010"},
            {"���ĵ�����", "ffffff-101010"},
            {"�������˶�", "ffffff-101010"},
            {"�淶ͻȻɢ", "ffffff-101010"},

            //-----------------------  ����  ----------------------------------------
            {NULL, NULL}, //������־,��Ҫ�޸�
        }
    },

    //�ֿ�2
    {
        FONT_TEAMGROUP_DIR"font-libraray2.txt",  
        {
            //-----------------------  ��ʼ  ----------------------------------------
            //�ֿ�2��Ҫ�ҵ��ּ�����ɫ,���16��Ҫ�ҵ���,�������������
            {"Ҫ�ҵ���1", "00ff00-101010"},
            {"Ҫ�ҵ���2", "00ff00-101010"},

            //-----------------------  ����  ----------------------------------------
            {NULL, NULL}, //������־,��Ҫ�޸�
        }
    },

    //TODO:�ֿ�3�Ͳο��ֿ�1����«��ư
    
};

static struct teammate_info_t __ti_info_1[] =
{
    //�ֿ�1
    {
        FONT_TEAMGROUP_DIR"font-libraray1-1.txt",
        {
            //-----------------------  ��ʼ  ----------------------------------------
            //�ֿ�1��Ҫ�ҵ��ּ�����ɫ,���16��Ҫ�ҵ���,�������������
            {"�Թⷴ��˼", "ffffff-101010"},
            {"���������", "ffffff-101010"},
            {"���ĵ�����", "ffffff-101010"},
            {"�������˶�", "ffffff-101010"},
            {"�淶ͻȻɢ", "ffffff-101010"},


            //-----------------------  ����  ----------------------------------------
            {NULL, NULL}, //������־,��Ҫ�޸�
        }
    },

    //�ֿ�2
    {
        FONT_TEAMGROUP_DIR"font-libraray2-1.txt",  
        {
            //-----------------------  ��ʼ  ----------------------------------------
            //�ֿ�2��Ҫ�ҵ��ּ�����ɫ,���16��Ҫ�ҵ���,�������������
            {"�����¹���", "00ff00-101010"},
            {"Ҫ�ҵ���2", "00ff00-101010"},

            //-----------------------  ����  ----------------------------------------
            {NULL, NULL}, //������־,��Ҫ�޸�
        }
    },

    //TODO:�ֿ�3�Ͳο��ֿ�1����«��ư
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static int lookupword(Idmsoft* dmobj, void* ctxt, rect_t* rect, int idx,
    struct teammate_info_t* ti, posxy_t* lposXY)
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
    char* font = NULL;
    struct teammate_t* name = NULL;
    
    font = ti->font;
    name = &ti->names[idx];
    
    dmobj->SetDict(0, font);
    code = dmobj->FindStrFast(rect->lt.x, rect->lt.y, rect->rb.x, rect->rb.y,
        name->word, name->color_offset, 0.9, &pposX, &pposY);
    dmobj->ClearDict(0);
    if (-1 == code)
    {
        return ERRNO_NOT_EXIST;
    }

    lposX = pposX.lVal;
    lposY = pposY.lVal;

    lposXY->x = lposX;
    lposXY->y = lposY;
    
    api_log_MsgDebug("%s:%s:%s:����:�ɹ�, lposX:%ld, lposY:%ld, x:%ld, y:%ld",
        name->word, name->color_offset, font, lposX, lposY, lposXY->x, lposXY->y);

    return 0;
}

static int inviate(Idmsoft* dmobj, void* ctxt)
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
    struct teammate_info_t* ti = NULL;
    struct teammate_info_t* ti1 = NULL;
    int selected_front_index = luaman_config->selected_front_index;
    char* font = NULL;
    struct teammate_t* name = NULL;
    posxy_t lposXY;
    rect_t rect;
    time_t startTime = time(NULL);
    time_t curTime = 0;
    double deltaTime = 0.0;
    int idx1st = 0;
    
    if (selected_front_index < 0 || selected_front_index >= LUAMAN_DIM(__ti_info))
    {
        api_log_MsgError("ti_info not match, selected_front_index:%d", selected_front_index);
        return ERRNO_INVALID;
    }
    
    ti = &__ti_info[selected_front_index];
    ti1 = &__ti_info_1[selected_front_index];
    font = ti->font;
    name = &ti->names[0];

    api_log_MsgDebug("����:���:��ʼ");

    rect.lt.x = 170;
    rect.lt.y = 170;
    rect.rb.x = 1279;
    rect.rb.y = 826;
    
    for (;;)
    {
        name = &ti->names[0];
        
        for (idx = 0; idx < LUAMAN_DIM(ti->names); idx++)
        {
            if (NULL == name->word || NULL == name->color_offset)
            {
                break;
            }

            LUAMAN_MEMSET(&lposXY, 0, sizeof(posxy_t));
            code = lookupword(dmobj, ctxt, &rect, idx, ti, &lposXY);
            if (ERRNO_SUCCESS == code)
            {
                idx1st = idx;
                
                api_log_MsgDebug("��һ��:����:�ɹ�, idx:%d, font:%s, word:%s, color_offset:%s",
                    idx, font, name->word, name->color_offset);
                goto next;
            }

            /*api_log_MsgDebug("lookupword:����:ʧ��, idx:%d, font:%s, word:%s, color_offset:%s",
                idx, font, name->word, name->color_offset);*/
            
            name++;
        }
    }
    
next:
    for (;;)
    {
        dmobj->MoveTo(lposXY.x + __ti_offset[0].x, lposXY.y + __ti_offset[0].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        code = lookupword(dmobj, ctxt, &__lookup_rect, idx1st, ti1, &lposXY);
        if (ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("���:�ڶ���:����:�ɹ�, idx:%d, font:%s, word:%s, color_offset:%s",
                idx, font, name->word, name->color_offset);
            break;
        }

        api_log_MsgDebug("���:�ڶ���:����:ʧ��, idx:%d, font:%s, word:%s, color_offset:%s",
            idx, font, name->word, name->color_offset);
        
        Delay(500);
    }

    //�����λ��
    for (;;)
    {
        dmobj->MoveTo(lposXY.x + __ti_offset[1].x, lposXY.y + __ti_offset[1].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT_TEAMGROUP_DIR"font-farteam.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "Զ����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("Զ����:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("Զ����:����:ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(lposX, lposY);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    Delay_With_Terminate_Check(60, luaman_task, dmobj);

    dmobj->KeyPressChar("o");
    Delay(500);
    dmobj->KeyUpChar("o");
    Delay(500);

    LUAMAN_MEMSET(&lposXY, 0, sizeof(posxy_t));
    startTime = time(NULL);
    for (;;)
    {
        code = lookupword(dmobj, ctxt, &rect, idx1st, ti, &lposXY);
        if (ERRNO_SUCCESS == code)
        {
            api_log_MsgDebug("������:����:�ɹ�, idx:%d, font:%s, word:%s, color_offset:%s, x:%ld, y:%ld",
                idx, font, name->word, name->color_offset, lposXY.x, lposXY.y);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, startTime);
        if (deltaTime > 300.0)
        {
            api_log_MsgDebug("������:����:��ʱ, idx:%d, font:%s, word:%s, color_offset:%s",
                idx, font, name->word, name->color_offset);
            return ERRNO_TIMEOUT;
        }

        api_log_MsgDebug("������:����:ʧ��, idx:%d, font:%s, word:%s, color_offset:%s",
            idx, font, name->word, name->color_offset);
        
        Delay(500);
    }

    lposX = lposXY.x;
    lposY = lposXY.y;

    api_log_MsgDebug("������:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
    
    //���
    for (;;)
    {
        dmobj->MoveTo(lposX + __ti_offset[2].x, lposY + __ti_offset[2].y);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        code = lookupword(dmobj, ctxt, &rect, idx1st, ti, &lposXY);
        if (ERRNO_SUCCESS != code)
        {
            api_log_MsgDebug("������:���:�ɹ�, idx:%d, font:%s, word:%s, color_offset:%s",
                idx, font, name->word, name->color_offset);
            break;
        }

        curTime = time(NULL);
        deltaTime = difftime(curTime, startTime);
        if (deltaTime > 300.0)
        {
            api_log_MsgDebug("������:���:��ʱ, idx:%d, font:%s, word:%s, color_offset:%s",
                idx, font, name->word, name->color_offset);
            return ERRNO_TIMEOUT;
        }

        api_log_MsgDebug("������:���:ʧ��, idx:%d, font:%s, word:%s, color_offset:%s, x:%ld, y:%ld",
            idx, font, name->word, name->color_offset, lposXY.x, lposXY.y);
        
        Delay(500);
    }

    dmobj->KeyPressChar("o");
    Delay(500);
    dmobj->KeyUpChar("o");
    Delay(500);
    
    return 0;
}

int api_TaskTeamGroupInit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    int code = 0;
    
    api_log_MsgDebug("����:TeamGroup:��ʼ��:��ʼ, hwnd:%ld", hwnd);

    /* �󶨴��� */
    code = dmobj->BindWindow(hwnd, "dx", "dx", "dx", 0);
    if (1 != code)
    {
        api_log_MsgError("bind TeamGroup hwnd failed, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("bind TeamGroup hwnd success, hwnd:%ld", hwnd);

    code = code = dmobj->LockInput(2);
    if (1 != code)
    {
        api_log_MsgError("lock TeamGroup hwnd failed, hwnd:%ld", hwnd);
        return ERRNO_BINDWND;
    }

    api_log_MsgDebug("����:TeamGroup:��ʼ��:���, hwnd:%ld", hwnd);

    /* ��ʼ���澯ʱ���ʼֵ */
    init_alarm_ctxt();
    
    return ERRNO_SUCCESS;
}

int api_TaskTeamGroupMain(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    ROLE_E role = luaman_config->role;
    int code = 0;
    int j = 0;
    
    api_log_MsgDebug("����:TeamGroup:��ѭ��: hwnd:%ld, role:%d", hwnd, (int)role);

    for (j = 0; j < luaman_config->loop_num; j++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
            
        api_log_MsgDebug("selected_front_index:%d", luaman_config->selected_front_index);
        code = inviate(dmobj, ctxt);
        if (code)
        {
            api_log_MsgDebug("ִ�и������,j:%ld, loop_num:%d", j, luaman_config->loop_num);
            break;
        }
    }

    api_log_MsgDebug("����:TeamGroup:��ѭ��:����, hwnd:%ld, role:%d", hwnd, (int)role);
    return ERRNO_SUCCESS;
}


int api_TaskTeamGroupExit(Idmsoft* dmobj, void* ctxt)
{
    struct clumantask* luaman_task = (struct clumantask*)ctxt;
    struct cluamantaskctxt* luaman_taskctxt = &luaman_task->ctxt;
    struct cluamanconfig* luaman_config = (struct cluamanconfig*)(luaman_taskctxt->context);
    long hwnd = luaman_task->hwnd;
    int groupid = luaman_config->groupid;
    ROLE_E role = luaman_config->role;
    char* roletips[] = ROLE_DESC_ARRAY();
    
    api_log_MsgDebug("����:TeamGroup:�˳�, hwnd:%ld, groupid:%d, role:%s",
        hwnd, groupid, roletips[role]);

    dmobj->LockInput(0);
    dmobj->UnBindWindow();
    
    return ERRNO_SUCCESS;
}

