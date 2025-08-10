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
#include "taskExSignle.h"
#include "snail_datatype.h"
#include "snail_errno.h"
#include "snail_log.h"
#include "snail_common.h"
#include "Markup.h"
#include "curl.h"
#include "taskExSignle_Pub.h"
#include "taskSignlePri.h"

//����Ҽ����ʱ��ֵ(��λ:����)
#define RIGHT_CLICK_DELAY_TIME  (500)

//�ֿ�����ƶ����ʱ��(��λ:����)
#define KEY_MOVE_DELAY_TIME  (30)

//�����·���ʱ��(��λ:����)
#define COAT_ON_DELAY_TIME  (30)

//�˺�-��ɫ-�ֿ����Ϣ
struct login_font_info_t lfi_pri[] =
{
    {"tzdk1601lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "�Ҹ���",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "�淶��",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "������",        FONT_EXSIGNLE_DIR"font-1-ZhangHao4-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1601lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},



    {"tzdk1602lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "������",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "�ɷ�һ",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "��ͨһ",        FONT_EXSIGNLE_DIR"font-1-ZhangHao2-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1602lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


    {"tzdk1603lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "ɳ����",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "��ͨ��",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "�����",        FONT_EXSIGNLE_DIR"font-1-ZhangHao3-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1603lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


    {"tzdk1604lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "����ɢ",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "������",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "ʮ����",        FONT_EXSIGNLE_DIR"font-1-ZhangHao1-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1604lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    


    {"tzdk1605lmq", "A",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "�ǵĶԷ�˼",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-B.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "C",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "������˼",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-D.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "E",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "F",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "G",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "H",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "I",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "J",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "K",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "�Ӻ���˼",        FONT_EXSIGNLE_DIR"font-1-ZhangHao5-L.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "M",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},
    {"tzdk1605lmq", "N",        FONT_EXSIGNLE_DIR"font-zimuA-N.txt", {"ffffff-101010", "76f856-101010"}},


};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//���������õ�����,�����޸�
/*
2 ������Ϸ֮���Ȱ�I��������Ρ��ڱ��������λ��1��pkgpos_relative��һ������������ȥ��
  �������ҵ�����Ӿ�£���ɫ������font-zhuangbei1.txt�������ҵ��ˣ��ص���������I���һسǾ��ᣬȻ������˳���ߡ�
*/
//����Ӿ����ɫ
#define SWIM_COAT_COLOR "00ff00-101010"

//���λ��
struct posxy_t pkgpos_relative[] =
{
    /*
    2 ������Ϸ֮���Ȱ�I��������Ρ��ڱ��������λ��1��pkgpos_relative��һ������������ȥ��
      �������ҵ�����Ӿ�£���ɫ������font-zhuangbei1.txt�������ҵ��ˣ��ص���������I���һسǾ��ᣬȻ������˳���ߡ�
    */
    {-68, 166}, //�������λ��
};

//�Ҳ��������ж����ꡣ��������ǣ�pos_info�����˷�Χ��㡣��ֱ�Ӱ�N�Ի���������ǣ��Ͱ�I���һسǾ�Ȼ��سǣ�Ѱ·
struct posxy_t pos_info[] =
{
    {304, 40},
};

/*
6 ���긱���ˣ��ص������ˣ�Ȼ��ȥ������349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|301:52|292:49|293:46��·��Ҫ�ģ��߼����䣩��

���������ˣ�Ѱ·292:49|301:52|308:54|305:48|304:43|304:40��Ȼ���Ȱ�װ������������I���ڱ��������λ�ã�16������
�ҵ��ֿ�1���ֿ�2���͵���Ҽ���Ȼ���ٵ��ɫ�� ���Ρ����ұ��������λ�ã�1������������Ӿ�£�font-zhuangbei1.txt��������Ҽ���
*/
//16������ڱ��������ƫ��ֵ
struct posxy_t coatoff_pos_info[] =
{
    //TODO:
    {-125, 155},
    {16,    93},
    {15,   185},
    {13,   279},
    {-128, 290},
    {157,  288},
    {-74,   71},
    {-56,  118},
    {87,   121},
    {-57,  221},
    {87,   220},
    {-55,  261},
    {86,   267},
    {-74,  361},
    {14,   360},
    {101,  361},

};

//���ұ��������λ�ã�1������������Ӿ�£�font-zhuangbei1.txt��������Ҽ���
//Ӿ����Ա�����λ��
struct posxy_t swimcoatoff_pos_info[] =
{
    //TODO:
    {-68, 166},

};

/*
7 ��������Ȼ��N�Ի��������ɫ�ģ�00ff00���ֿ��š�font-cunfang1.txt��Ȼ����ɫ�ģ�929292�����á�Ȼ�󱳰�����һ�顣
�����ֿ�1�ģ��͵���Ҽ��������ֿ�2�ģ����ĸ�������������Ȼ���ٰ�����ƶ�����Ӧ��λ�ã��ٵ�������Ͱ����ĸ�Ҳ�Ųֿ����ˡ�
*/
//�����ֿ�2�ģ����ĸ�������ڹ��õ�λ��
struct posxy_t mov2posIdx_pos_info[] =
{
    //TODO:
    {-117, 231},
    {-58,  228},
    {7,    230},
    {70,   231},

};

/*
2 ������Ϸ֮���Ȱ�I���������(font-FuShi.txt)(929292)���ڱ��������λ��1��pkgpos_relative��һ������������ȥ��
  �������ҵ�����Ӿ�£���ɫ������font-zhuangbei1.txt�������ҵ��ˣ��ص���������I���һسǾ��ᣬȻ������˳���ߡ�

  �Ҳ��������ж����ꡣ��������ǣ�pos_info�����˷�Χ��㡣��ֱ�Ӱ�N�Ի���������ǣ��Ͱ�I���һسǾ�Ȼ��سǣ�Ѱ·������
*/
#define WALK_PATH_FRIST()   "349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|305:48|304:43|304:40"

// ����11�����ֿ�1
struct font_info_t library_font_array_1[] =
{
    {"Ӱ֮����",         FONT_EXSIGNLE_DIR"font-ziku1-Tou.txt",       "a335ee-101010"},
    {"����Ӿ��",         FONT_EXSIGNLE_DIR"font-ziku1-ShiZhuang.txt", "ffffff-101010"},
    {"�������ֻ�",        FONT_EXSIGNLE_DIR"font-ziku1-ChangShou.txt", "00ff00-101010"},
    {"ʯ��ʯ����",        FONT_EXSIGNLE_DIR"font-ziku1-ShiLiu.txt",    "00ff00-101010"},
    {"����",           FONT_EXSIGNLE_DIR"font-ziku1-ShouTao.txt",   "ffe08c-101010"},
    {"�ڰ�����",         FONT_EXSIGNLE_DIR"font-ziku1-YaoDai.txt",    "00ff00-101010"},
    {"���",         FONT_EXSIGNLE_DIR"font-ziku1-YanWang.txt",   "ffe08c-101010"},
    {"ӡ�ڰ�����",        FONT_EXSIGNLE_DIR"font-ziku1-YinDiAn.txt",   "ffffff-101010"},
    {"�����׵Ķ���", FONT_EXSIGNLE_DIR"font-ziku1-HaiErHuan.txt", "a335ee-101010"},
    {"������Ԩ",         FONT_EXSIGNLE_DIR"font-ziku1-HaiJieZhi.txt", "a335ee-101010"},
    {"����ָ",         FONT_EXSIGNLE_DIR"font-ziku1-HunJie.txt",    "ffffff-101010"},

    //TODO:����������,����«��ư
    
};

//�����ĸ����ֿ�2
struct font_info_t library_font_array_2[] =
{
    {"�����˫Ҷ��", FONT_EXSIGNLE_DIR"font-ziku2-ShuangYeDao.txt", "a335ee-101010"},
    {"���������",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnTui.txt",   "a335ee-101010"},
    {"�����սѥ",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnXue.txt",   "a335ee-101010"},
    {"���������",          FONT_EXSIGNLE_DIR"font-ziku2-HeLiEnJia.txt",   "a335ee-101010"},

    //TODO:����������,����«��ư
    
};

struct font_info_t library_font_array_3[] =
{
    {"������Ƭ", FONT_EXSIGNLE_DIR"font-ziku3-kuangqi.txt", "ffffff-101010"},
    {"������Ƭ",     FONT_EXSIGNLE_DIR"font-ziku3-beifen.txt",  "ffffff-101010"},

    //TODO:����������,����«��ư
    
};


//�ֿ�����Ϊ:"font-exclrpkg2.txt"
static gabage_info usrdef_gabage_info[] = 
{
    //{"����1", "00ff00-010101"}, //һ�б�ʾһ������
    //{"����2", "00ff00-010101"},
    //TODO:�ο�����1������2д,'//'��ʾע��
    {"��������", "ffffff-010101"},
    {"��������", "c1c1ff-010101"},
    {"��������", "ffe08c-010101"},
    
    
};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!! �������ݲ�Ҫ�޸� !!!!!!!!!!!!!!!!!!!
#define FONTSIGNLE_DIR "font\\exsignle\\"
#define BMPSIGNLE_DIR "bmp\\exsignle\\"

static int enter_with_role(Idmsoft* dmobj, void* ctxt)
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
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    char* user_name = luaman_config->user_name;
    int roleno = luaman_config->roleno;
    char* role_name = NULL;
    int i = 0, j = 0;
    int clicknr = 0;
    struct posxy_t posxys[] = {
        {1219, 608},
        {1219, 608},
        {1219, 608},
        {1105, 607},
        {1105, 607},
        {1105, 607},
    };
        
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(500);

        for (i = 0; i < LUAMAN_DIM(lfi_pri); i++)
        {
            if (strcmp(lfi_pri[i].login, user_name))
            {
                api_log_MsgDebug("�˺�:%s:ƥ��:ʧ��", user_name);
                continue;
            }

            role_name = lfi_pri[i + roleno].word;
            api_log_MsgDebug("�˺�:%s:��ɫ:%s:ƥ��:�ɹ�", user_name, role_name);

            for (j = 0; j < LUAMAN_DIM(lfi_pri[i + roleno].color_offset); j++)
            {
                dmobj->SetDict(0, lfi_pri[i + roleno].font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, role_name, lfi_pri[i + roleno].color_offset[j], 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:��ɫ:%s:����:�ɹ�, lposX:%ld, lposY:%ld",
                        user_name, roleno, role_name, lfi_pri[i + roleno].color_offset[j],lposX, lposY);
                    goto label_click_role;
                }

                api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:�ֿ�:%s:��ɫ:%s:����:ʧ��",
                    user_name, roleno, role_name, lfi_pri[i + roleno].font,
                    lfi_pri[i + roleno].color_offset[j]);
            }

            api_log_MsgDebug("�˺�:%s:��ɫ:%s:����:ʧ��", user_name, role_name);
            break;
        }

        //��������μ�ͷ��ҳ�ټ�������
        dmobj->MoveTo(posxys[clicknr].x, posxys[clicknr].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(500);
    
        clicknr = (clicknr + 1) % LUAMAN_DIM(posxys);
        
        api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:����:ʧ��:��ҳ", user_name, roleno, role_name);
        Delay(500);
    }

label_click_role:
    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role0.bmp");
    
    //���ѡ���ɫ
    dmobj->MoveTo(lposX, lposY);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role1.bmp");
    
    /////////////////////////////////////////////////////////////////////////////////
    //���������Ϸ
    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    return SNAIL_ERRNO_SUCCESS;
}

static int Buy_Blue(Idmsoft* dmobj, void* ctxt)
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
    code = dmobj->SendString(hwnd, "300");
    if (1 != code)
    {
        api_log_MsgError("�̵�:���:300:����:ʧ��");
        return ERRNO_IGNORE;
    }

    api_log_MsgDebug("����:���:300:����:�ɹ�");

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

static int sell_gabage(Idmsoft* dmobj, void* ctxt)
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
    int findout = 0;
    
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

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
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

                findout = 0;
                
                for (idx = 0; idx < sizeof(usrdef_gabage_info)/sizeof(usrdef_gabage_info[0]); idx++)
                {
                    dmobj->SetDict(0, FONTSIGNLE_DIR"font-exclrpkg2.txt");
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

                    findout = 1;
                    break;
                }

                if (findout)
                {
                    continue;
                }

                dmobj->SetDict(0, FONTSIGNLE_DIR"font-ziku3-yan.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "��", "ffffff-101010", 0.9, &pposX1, &pposY1);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                api_log_MsgDebug("���:����:%d:����:[%d:%d]:����:%d:��:�ɹ�", lndex, index, jndex, idx);
                
                dmobj->SetDict(0, FONTSIGNLE_DIR"font-beasellman.txt");
                code = dmobj->FindStrFast(0, 27, 1279, 826, "180", "ffe08c-101010", 0.9, &pposX2, &pposY2);
                dmobj->ClearDict(0);
                if (0 != code)
                {
                    continue;
                }

                api_log_MsgDebug("���:����:%d:����:[%d:%d]:����:%d:180:�ɹ�", lndex, index, jndex, idx);
                
                Delay(200);
                dmobj->RightClick();
                Delay(300);
                dmobj->KeyDownChar("enter");
                Delay(300);
                dmobj->KeyUpChar("enter");
                Delay(300);
            }
        }
    }

    //���� 2023/5/17 22:00:00
    (void)Buy_Blue(dmobj, ctxt);
    
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

static int clean_package(Idmsoft* dmobj, void* ctxt)
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
    
    pathofposxys = "349:129|353:119|355:114|357:107|356:97|351:91|345:86|338:81|332:78|325:75|319:71|315:66|312:61|308:54|301:52|292:49|283:46";
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[293:46]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[293:46]:�ɹ�");
    
    //��n
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->KeyPressChar("n");
        Delay(500);
        dmobj->KeyUpChar("n");
        Delay(500);

        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "��ʿ(����)", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;
        
            api_log_MsgDebug("��ʿ(����):����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("��ʿ(����):����:ʧ��");
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
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
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

        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-QingBao1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����:���:�ɹ�");
            break;
        }
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;

        api_log_MsgDebug("����:���:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }
    
    return sell_gabage(dmobj, ctxt);
}

static int coatoff(Idmsoft* dmobj, void* ctxt)//���·�
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
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int fintout = 0;
    
    api_log_MsgDebug("����:����:��ʼ");

    dmobj->MoveTo(5, 5);
    Delay(300);

    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);

        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);
        
        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONTSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    for (index = 0; index < LUAMAN_DIM(coatoff_pos_info); index++)
    {
        posX = lposX2 + coatoff_pos_info[index].x;
        posY = lposY2 + coatoff_pos_info[index].y;
        
        dmobj->MoveTo(posX, posY);
        Delay(RIGHT_CLICK_DELAY_TIME);

        dmobj->RightClick();
        Delay(RIGHT_CLICK_DELAY_TIME);
    }

    api_log_MsgDebug("����:����:����");
    return SNAIL_ERRNO_SUCCESS;
}

static int coaton(Idmsoft* dmobj, void* ctxt)//���·�
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
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int fintout = 0;
    
    api_log_MsgDebug("����:����:��ʼ");

    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);

        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);
        
        //���ҳ��ֱ������������ɹ�
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
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

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
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

    for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

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
                Delay(COAT_ON_DELAY_TIME);

                fintout = 0;
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_1); idx++)
                {
                    font = library_font_array_1[idx].font;
                    word = library_font_array_1[idx].word;
                    color_offset = library_font_array_1[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:�ֿ�1:����:�ɹ�, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        dmobj->RightClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }

                for (idx = 0; idx < LUAMAN_DIM(library_font_array_2); idx++)
                {
                    font = library_font_array_2[idx].font;
                    word = library_font_array_2[idx].word;
                    color_offset = library_font_array_2[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:�ֿ�2:����:�ɹ�, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        dmobj->RightClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);
                        
                        break;
                    }
                }
            }
        }
    }

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    Check_Terminate_Thread(luaman_task, dmobj);

    Check_Terminate_Thread(luaman_task, dmobj);
    
    api_log_MsgDebug("����:����:���");
    return SNAIL_ERRNO_SUCCESS;
}

//��Ӿ��
static int swimcoatoff(Idmsoft* dmobj, void* ctxt)
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
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 0, deltaY = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    int index = 0;

    api_log_MsgDebug("Ӿ��:����:��ʼ");
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    //������� 880:121 -> 876:151
    dmobj->MoveTo(lposX2 - 4, lposY2 + 30);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);

    for (index = 0; index < LUAMAN_DIM(swimcoatoff_pos_info); index++)
    {
        posX = lposX2 + swimcoatoff_pos_info[index].x;
        posY = lposY2 + swimcoatoff_pos_info[index].y;
        
        dmobj->MoveTo(posX, posY);
        Delay(RIGHT_CLICK_DELAY_TIME);

        dmobj->RightClick();
        Delay(RIGHT_CLICK_DELAY_TIME);
    }

    api_log_MsgDebug("Ӿ��:����:����");
    return SNAIL_ERRNO_SUCCESS;
}

static int back2library(Idmsoft* dmobj, void* ctxt)
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
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 35;
    long deltaY = 35;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    int index = 0;
    long buildx = 0, buildy = 0;
    int lndex = 0;
    int jndex = 0;
    int fintout = 0;
    int idx = 0;
    struct xyposition boxPosXYs[5 * 10];
    long boxPosX0 = 0;
    long boxPosY0 = 0;
    long boxPosX1 = 0;
    long boxPosY1 = 0;
    struct xyposition labelPosXY[5];
    int mov2posIdx = 0;
    long  lineX = -1;
    long  lineY = -1;
    
    api_log_MsgDebug("�黹:�ֿ�:���");

    dmobj->MoveTo(5, 5);
    Delay(100);
        
    //��i��������
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("i");
        Delay(200);
        dmobj->KeyUpChar("i");
        Delay(200);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("����:����,�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);

            lposX2 = lposX;
            lposY2 = lposY;
            
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

        api_log_MsgDebug("����:����,ʧ��");
        Delay(500);
    }

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX, lposY);
    
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
        }
    }
    
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
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ֿ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ֿ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        api_log_MsgDebug("�ֿ���:����:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(500);
    }

    //����ֿ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (lndex = 0; lndex < 2; lndex++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        
        /*if (!CHECK_CLEAN_PACKAGE_X(cleanPackageX, lndex + 1))
        {
            continue;
        }*/

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
                Delay(50);

                fintout = 0;
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_1); idx++)
                {
                    font = library_font_array_1[idx].font;
                    word = library_font_array_1[idx].word;
                    color_offset = library_font_array_1[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:�ֿ�1:����:�ɹ�, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->RightClick();
                        Delay(20);

                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }

                for (idx = 0; idx < LUAMAN_DIM(library_font_array_2); idx++)
                {
                    font = library_font_array_2[idx].font;
                    word = library_font_array_2[idx].word;
                    color_offset = library_font_array_2[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("װ��%d:��Ԫ��[%d:%d]:����:%s:�ֿ�2:����:�ɹ�, lposX:%ld, lposY:%ld",
                            mov2posIdx, index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->LeftClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);

                        //�Ƶ�������ڹ��õ�ƫ��λ��
                        lposX = lposX2 + mov2posIdx_pos_info[mov2posIdx].x;
                        lposY = lposY2 + mov2posIdx_pos_info[mov2posIdx].y;

                        dmobj->MoveTo(lposX, lposY);
                        Delay(20);

                        dmobj->LeftClick();
                        Delay(RIGHT_CLICK_DELAY_TIME);
                        
                        mov2posIdx++;
                        fintout = 1;
                        break;
                    }
                }

                if (fintout)
                {
                    continue;
                }
                
                for (idx = 0; idx < LUAMAN_DIM(library_font_array_3); idx++)
                {
                    font = library_font_array_3[idx].font;
                    word = library_font_array_3[idx].word;
                    color_offset = library_font_array_3[idx].color_offset;
                
                    dmobj->SetDict(0, font);
                    code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                    dmobj->ClearDict(0);
                    if (-1 != code)
                    {
                        lposX = pposX.lVal;
                        lposY = pposY.lVal;

                        api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:�ֿ�3:����:�ɹ�, lposX:%ld, lposY:%ld", index, jndex, word, lposX, lposY);

                        dmobj->MoveTo(posX, posY);
                        Delay(20);

                        dmobj->RightClick();
                        Delay(20);
                        break;
                    }
                }
            }
        }
    }
    
    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);

    dmobj->KeyDownChar("esc");
    Delay(500);
    dmobj->KeyUpChar("esc");
    Delay(500);
    
    api_log_MsgDebug("�黹:�ֿ�:���");
    return SNAIL_ERRNO_SUCCESS;
}

static int do_work(Idmsoft* dmobj, void* ctxt, int dotask_num)
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
    int i = 0, j = 0, k = 0;
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    long posX = 0, posY = 0;
    CString pathOfPosXYs;
    char* pathofposxys = NULL;
    int result = 0;
    long posX0 = 0, posY0 = 0, posX1 = 0, posY1 = 0;
    long deltaX = 0, deltaY = 0;
    char* word = NULL;
    char* font = NULL;
    char* color_offset = NULL;
    int findout = 0;
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(200);
        
        dmobj->KeyDownChar("i");
        Delay(500);
        dmobj->KeyUpChar("i");
        Delay(500);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;

            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //�������
    do_click_tidy_package(luaman_task, dmobj, lposX2, lposY2);
    
    //������� 880:121 -> 876:151
    dmobj->MoveTo(lposX2 - 4, lposY2 + 30);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    
    //�ƶ����������λ�õ��
    dmobj->MoveTo(lposX2 + pkgpos_relative[0].x, lposY2 + pkgpos_relative[0].y);
    Delay(200);
    
    for (i = 0; i < 4; i++)
    {
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-ziku1-ShiZhuang.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����Ӿ��", "ffffff-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            result = 1;
            
            lposX = pposX.lVal;
            lposY = pposY.lVal;
            
            api_log_MsgDebug("����Ӿ��:%s:����:�ɹ�, lposX:%ld, lposY:%ld", SWIM_COAT_COLOR, lposX, lposY);
            break;
        }

        api_log_MsgDebug("����Ӿ��:%s:����:ʧ��", SWIM_COAT_COLOR);
        Delay(500);
    }

    dmobj->MoveTo(5, 5);
    Delay(200);
        
    //�ص�����
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->KeyDownChar("i");
        Delay(300);
        dmobj->KeyUpChar("i");
        Delay(300);

        //����Ƿ񰴳ɹ�
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-package.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����:�ر�:�ɹ�");
            break;
        }

        lposX2 = pposX2.lVal;
        lposY2 = pposY2.lVal;

        api_log_MsgDebug("����:�ر�:ʧ��, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
        Delay(500);
    }
    
    if (result)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        status = signle_pri_clickhome(dmobj, ctxt);
        if (SNAIL_ERRNO_SUCCESS != status)
        {
            api_log_MsgError("�س�:ʧ��:status:%d", status);
            return status;
        }

        api_log_MsgDebug("�س�:�ɹ�");
        goto do_copy_work;
    }

    //�������귶Χ
    for (i = 0; i < 4; i++)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(5, 5);
        Delay(300);

        //����Ƿ񵽴�Ŀ������
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-posxy.txt");
        strPosXY = dmobj->Ocr(1166, 62, 1222, 74, "ffffff-101010", 0.9);
        dmobj->ClearDict(0);
        if (strPosXY != CString(""))
        {
            spcnt = SplitStringWide(strPosXY, ":", posXY);
            if (spcnt >= 2)
            {
                posX = atol(posXY.GetAt(0));
                posY = atol(posXY.GetAt(1));

                if (abs(posX - pos_info[0].x) <= 3 && abs(posY - pos_info[0].y) <= 3)
                {
                    api_log_MsgDebug("����:%ld:%ld:����:�ɹ�", pos_info[0].x, posY - pos_info[0].y);
                    goto do_dialog;
                }

                api_log_MsgDebug("����:%ld:%ld:����:ʧ��", pos_info[0].x, posY - pos_info[0].y);
            }
        }

        Delay(500);
    }

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("�س�:�ɹ�");

    //Ѱ·
    pathofposxys = WALK_PATH_FRIST();
    pathOfPosXYs = CString(pathofposxys);
    code = signle_pri_walk(dmobj, ctxt, pathOfPosXYs, luaman_config->direction);
    if (SNAIL_ERRNO_REALIVE == code || SNAIL_ERRNO_BLOCKED == code)
    {
        api_log_MsgDebug("Ѱ·:ǰ��:����:[???]:����:����");
        return SNAIL_ERRNO_REALIVE;
    }

    api_log_MsgDebug("Ѱ·:ǰ��:����:[???]:�ɹ�");
    api_log_MsgDebug("��ʼ:�Ի�");
    
do_dialog:
    dmobj->MoveTo(5, 5);
    Delay(200);
    
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
            
        dmobj->KeyDownChar("n");
        Delay(200);
        dmobj->KeyUpChar("n");
        Delay(200);

        dmobj->SetDict( 0, FONT_EXSIGNLE_DIR"font-cunfang1.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ֿ���", "00ff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX = pposX.lVal;
            lposY = pposY.lVal;

            api_log_MsgDebug("�ֿ���:����:�ɹ�, lposX:%ld, lposY:%ld", lposX, lposY);
            break;
        }

        Delay(500);
        api_log_MsgDebug("�ֿ���:����:ʧ��");
    }

    //����ֿ���
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-cunfang3.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����", "929292-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
        
            api_log_MsgDebug("����:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        api_log_MsgDebug("����:����:ʧ��");
        Delay(500);
    }

    //�ֿ� 630:146 -> ���� 620:170
    dmobj->MoveTo(lposX2, lposY2);
    Delay(200);
    dmobj->LeftClick();
    Delay(200);
    dmobj->MoveTo(5, 5);
    Delay(200);

    //�������ڸ��� 620:170 -> 480:190 -> 800:510
    posX0 = lposX2 - 140;
    posY0 = lposY2 + 20;
    posX1 = lposX2 + 180;
    posY1 = lposY2 + 340;
    deltaX = (posX1 - posX0) / 10;
    deltaY = (posY1 - posY0) / 10;

    posX0 = posX0 + deltaX / 2;
    posY0 = posY0 + deltaY / 2;

    api_log_MsgDebug("deltaX  : %ld", deltaX);
    api_log_MsgDebug("deltaY  : %ld", deltaY);
    api_log_MsgDebug("posX0   : %ld", posX0);
    api_log_MsgDebug("posY0   : %ld", posY0);
    
    for (i = 0; i < 10; i++)
    {
        if (i > 2 && i != 6)
        {
            continue;
        }
        
        posY = posY0 + i * deltaY;

        for (j = 0; j < 10; j++)
        {
            posX = posX0 + j * deltaX;

            dmobj->MoveTo(posX, posY);
            Delay(KEY_MOVE_DELAY_TIME);

            findout = 0;
            
            for (k = 0; k < LUAMAN_DIM(library_font_array_1); k++)
            {
                font = library_font_array_1[k].font;
                word = library_font_array_1[k].word;
                color_offset = library_font_array_1[k].color_offset;
            
                dmobj->SetDict(0, font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:����:�ֿ�1:�ɹ�, lposX:%ld, lposY:%ld", i, j, word, lposX, lposY);

                    dmobj->MoveTo(posX, posY);
                    Delay(KEY_MOVE_DELAY_TIME);

                    dmobj->RightClick();
                    Delay(KEY_MOVE_DELAY_TIME);

                    findout = 1;
                    break;
                }
            }

            if (findout)
            {
                continue;//���ü���������
            }
            
            for (k = 0; k < LUAMAN_DIM(library_font_array_2); k++)
            {
                font = library_font_array_2[k].font;
                word = library_font_array_2[k].word;
                color_offset = library_font_array_2[k].color_offset;
            
                dmobj->SetDict(0, font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, word, color_offset, 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("װ��:��Ԫ��[%d:%d]:����:%s:�ֿ�2:����:�ɹ�, lposX:%ld, lposY:%ld", i, j, word, lposX, lposY);

                    dmobj->MoveTo(posX, posY);
                    Delay(KEY_MOVE_DELAY_TIME);

                    dmobj->RightClick();
                    Delay(KEY_MOVE_DELAY_TIME);
                    
                    break;
                }
            }
        }
    }

    //��2��ESC
    dmobj->KeyDownChar("ESC");
    Delay(200);
    dmobj->KeyUpChar("ESC");
    Delay(200);
    dmobj->KeyDownChar("ESC");
    Delay(200);
    dmobj->KeyUpChar("ESC");
    Delay(200);

    status = signle_pri_clickhome(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgError("�س�:ʧ��:status:%d", status);
        return status;
    }

    api_log_MsgDebug("�س�:�ɹ�");
    
    status = coaton(dmobj, ctxt);
    if (SNAIL_ERRNO_SUCCESS != status)
    {
        api_log_MsgDebug("����:����:ʧ��, status:%d", status);
        return status;
    }

    api_log_MsgDebug("����:����:�ɹ�");

do_copy_work:
    return signle_pri_copy(dmobj, ctxt, dotask_num);
}

int start_work(struct cluamanaccountinfo* accountinfo, int roleno, Idmsoft* dmobj, void* ctxt)
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
    CString strPosXY;
    CStringArray posXY;
    int spcnt = 0;
    int i = 0;
    struct passwd2{char ch;char* path;} bmps[]=
    {
        {'0', BMP_EXSIGNLE_DIR"0.bmp"},
        {'1', BMP_EXSIGNLE_DIR"1.bmp"},
        {'2', BMP_EXSIGNLE_DIR"2.bmp"},
        {'3', BMP_EXSIGNLE_DIR"3.bmp"},
        {'4', BMP_EXSIGNLE_DIR"4.bmp"},
        {'5', BMP_EXSIGNLE_DIR"5.bmp"},
        {'6', BMP_EXSIGNLE_DIR"6.bmp"},
        {'7', BMP_EXSIGNLE_DIR"7.bmp"},
        {'8', BMP_EXSIGNLE_DIR"8.bmp"},
        {'9', BMP_EXSIGNLE_DIR"9.bmp"},
    };
    int j = 0;
    char* path = NULL;
    int dotask_num = 0;
    time_t check_conn_start;
    time_t check_conn_stop;
    double check_conn_deltaTime = 0.0;
    //��ɫ��������={A?, B?, C?,...,}
    //char* role_names[] = EXSIGNLE_ROLES();
    char* role_name = NULL;
    struct posxy_t posxys[] = {
        {1219, 608},
        {1219, 608},
        {1219, 608},
        {1105, 607},
        {1105, 607},
        {1105, 607},
    };

    luaman_config->roleno = roleno;
    luaman_config->user_name = accountinfo->user_name;
    luaman_config->enter_with_role = enter_with_role;
    
    if (roleno >= EXSIGNLE_ROLES_NR || roleno < 0)
    {
        api_log_MsgError("����:����:��ɫ:%d:����:����", roleno);
        return SNAIL_ERRNO_INVALID;
    }

    //role_name = role_names[roleno];
    api_log_MsgDebug("����:��ɫ:����:ִ��:��ʼ");
    api_log_MsgDebug("user       : %s", accountinfo->user_name);
    api_log_MsgDebug("passwd     : %s", accountinfo->passwd);
    api_log_MsgDebug("passwdex   : %s", accountinfo->passwdex);
    api_log_MsgDebug("roleno     : %d", roleno);
    //api_log_MsgDebug("role_name  : %s", role_name);
    
label_login:
    dmobj->MoveTo(5, 5);
    Delay(200);

    check_conn_start = time(NULL);
    
    //���ҡ��ʺš�
    //for (;;)
    for_timeout_check_start(luaman_task)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-login.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "�ʺ�", "ffffff-101010", 0.9, &pposX2, &pposY2);
        dmobj->ClearDict(0);
        if (-1 != code)
        {
            lposX2 = pposX2.lVal;
            lposY2 = pposY2.lVal;
                
            api_log_MsgDebug("�ʺ�:����:�ɹ�, lposX2:%ld, lposY2:%ld", lposX2, lposY2);
            break;
        }

        for_timeout_check(luaman_task, FOR_CHECK_TIMEOUT_VAL);
        api_log_MsgDebug("�ʺ�:����:ʧ��");
        Delay(500);

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
    }
    for_timeout_check_end()
    
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
        check_alarm(dmobj, ctxt);
        
        //dmobj->MoveTo(509, 730);
        dmobj->MoveTo(lposX2 + 50, lposY2 + 72);
        Delay(200);
        dmobj->LeftClick();
        Delay(200);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-login.txt");
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

    check_conn_start = time(NULL);
    
    //����������
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
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
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-loginerr.txt");
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
                check_alarm(dmobj, ctxt);
                
                dmobj->MoveTo(lposX + 117, lposY + 291);
                Delay(500);
                dmobj->LeftClick();
                Delay(500);
                dmobj->MoveTo(5, 5);
                Delay(200);

                dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-loginerr.txt");
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

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
        
        api_log_MsgDebug("����:����:����:ʧ��");
        Delay(1000);
    }

    for (i = 0; i < strlen(accountinfo->passwdex); i++)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
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
            check_alarm(dmobj, ctxt);
            
            dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
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

    check_conn_start = time(NULL);
    
    //���ȷ��,����������������: 586:266 -> 686:430
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX0 + 100, lposY0 + 164);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);
    
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-security.txt");
        code = dmobj->FindStrFast(0, 27, 1279, 826, "����������������", "ffff00-101010", 0.9, &pposX, &pposY);
        dmobj->ClearDict(0);
        if (-1 == code)
        {
            api_log_MsgDebug("����:����:ȷ��:�ɹ�");
            break;
        }

        check_conn_stop = time(NULL);
        check_conn_deltaTime = difftime(check_conn_stop, check_conn_start);
        
        
        lposX = pposX.lVal;
        lposY = pposY.lVal;
                
        api_log_MsgDebug("����:����:ȷ��:ʧ��, lposX:%ld, lposY:%ld", lposX, lposY);
        Delay(2000);
    }
    
    api_log_MsgDebug("����:��Ϸ:�ɹ�:׼��:��ʼ");

    Delay_With_Terminate_Check(5, luaman_task, dmobj);

    int clicknr = 0;
    
    /////////////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        Check_Terminate_Thread_With_Alram(luaman_task, dmobj);
        dmobj->MoveTo(5, 5);
        Delay(500);

        for (i = 0; i < LUAMAN_DIM(lfi_pri); i++)
        {
            if (strcmp(lfi_pri[i].login, accountinfo->user_name))
            {
                api_log_MsgDebug("�˺�:%s:ƥ��:ʧ��", accountinfo->user_name);
                continue;
            }

            role_name = lfi_pri[i + roleno].word;
            
            api_log_MsgDebug("�˺�:%s:��ɫ:%s:ƥ��:�ɹ�", accountinfo->user_name, role_name);

            for (j = 0; j < LUAMAN_DIM(lfi_pri[i + roleno].color_offset); j++)
            {
                dmobj->SetDict(0, lfi_pri[i + roleno].font);
                code = dmobj->FindStrFast(0, 27, 1279, 826, role_name, lfi_pri[i + roleno].color_offset[j], 0.9, &pposX, &pposY);
                dmobj->ClearDict(0);
                if (-1 != code)
                {
                    lposX = pposX.lVal;
                    lposY = pposY.lVal;

                    api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:��ɫ:%s:����:�ɹ�, lposX:%ld, lposY:%ld",
                        accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].color_offset[j],lposX, lposY);
                    goto label_click_role;
                }

                api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:�ֿ�:%s:��ɫ:%s:����:ʧ��",
                    accountinfo->user_name, roleno, role_name, lfi_pri[i + roleno].font,
                    lfi_pri[i + roleno].color_offset[j]);
            }

            api_log_MsgDebug("�˺�:%s:��ɫ:%s:����:ʧ��", accountinfo->user_name, role_name);
            break;
        }

        //��������μ�ͷ��ҳ�ټ�������
        dmobj->MoveTo(posxys[clicknr].x, posxys[clicknr].y);
        Delay(300);
        dmobj->LeftClick();
        Delay(300);
        dmobj->MoveTo(5, 5);
        Delay(500);
    
        clicknr = (clicknr + 1) % LUAMAN_DIM(posxys);
        
        api_log_MsgDebug("�˺�:%s:��ɫ:%d:����:%s:����:ʧ��:��ҳ", accountinfo->user_name, roleno, role_name);
        Delay(500);
    }

label_click_role:
    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role0.bmp");
    
    //���ѡ���ɫ
    dmobj->MoveTo(lposX, lposY);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);

    //dmobj->Capture(0, 27, 1279, 826, "cap\\sel_role1.bmp");
    
    /////////////////////////////////////////////////////////////////////////////////
    //���������Ϸ
    dmobj->MoveTo(640, 670);
    Delay(500);
    dmobj->LeftClick();
    Delay(500);
    dmobj->MoveTo(5, 5);
    Delay(200);

    i = 0;
    
next:
    Delay_With_Terminate_Check(3, luaman_task, dmobj);
    
    dotask_num = 0;
    
    //ִ�С����񸱱���
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        check_alarm(dmobj, ctxt);
        
        code = do_work(dmobj, ctxt, dotask_num);
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

    //���
    (void)clean_package(dmobj, ctxt);

    //Ѱ·
    (void)signle_pri_postwalk(dmobj, ctxt);

    //���·�
    (void)coatoff(dmobj, ctxt);

    //��Ӿ��
    (void)swimcoatoff(dmobj, ctxt);

    (void)back2library(dmobj, ctxt);

    //�˳�����
    for (;;)
    {
        Check_Terminate_Thread(luaman_task, dmobj);
        code = check_alarm(dmobj, ctxt);
        if (ERRNO_ALARM == code)
        {
            Delay(1000);
            api_log_MsgDebug("׼��:���µ�¼:����:���:�ɹ�");
            continue;
        }
        
        //��� 1262:747
        dmobj->MoveTo(1262, 747);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-logout.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->MoveTo(lposX, lposY);
        Delay(500);
        dmobj->LeftClick();
        Delay(500);
        dmobj->MoveTo(5, 5);
        Delay(200);

        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-saveconf.txt");
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
        check_alarm(dmobj, ctxt);
        
        dmobj->SetDict(0, FONT_EXSIGNLE_DIR"font-saveconf.txt");
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

static void __this_null(void){}

