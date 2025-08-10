// Param000Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "xrrno.h"
#include "luaman.h"
#include "config.h"
#include "datatype.h"
#include "Param1853Dlg.h"
#include "Param195Dlg.h"
#include "Param021Dlg.h"
#include "ParamSigninDialog.h"
#include "ParamSakuraDlg.h"
#include "ParamDotaskDlg.h"
#include "Param000Dlg.h"
#include "ParamFightDlg.h"
#include "ParamSignleDlg.h"
#include "ParamRefugeboatDlg.h"
#include "ParamExsignleDlg.h"
#include "ParamEx175Dlg.h"
#include "ParamTeamGroupDlg.h"
#include <afxtempl.h> //CArray头文件
#include <string.h>
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParam000Dlg dialog
#define LUAMAN_WNDLIST() {"游戏窗口-1", "游戏窗口-2", "游戏窗口-3", "游戏窗口-4", "游戏窗口-5"}
#define LUAMAN_HOSTLIST() {"主机-1", "主机-2", "主机-3", "主机-4", "主机-5",\
    "主机-6", "主机-7", "主机-8", "主机-9", "主机-10",\
    "主机-11", "主机1-2", "主机-13", "主机-14", "主机-15",\
    "主机-16", "主机-17", "主机-18", "主机-19", "主机-20",\
}


CParam000Dlg::CParam000Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParam000Dlg::IDD, pParent), CParameters()
{
	//{{AFX_DATA_INIT(CParam000Dlg)
	//}}AFX_DATA_INIT
#ifdef HAVE_PARAM000_REIMPL
	INIT_LIST_HEAD(&m_config.node);
    INIT_LIST_HEAD(&m_config.accountList);
    INIT_LIST_HEAD(&m_config.mixConfigList);
#endif
    m_config.taskType = TASK_TYPE_000;
}

#ifdef HAVE_PARAM000_REIMPL
int CParam000Dlg::SetConfig(struct cluamanconfig* config)
{
    int status = api_CloneConfig(&m_config, config);
    if (ERRNO_SUCCESS == status)
    {
        api_log_MsgDebug("init dlg000 config success");
        
        //api_PrintConfig(&m_config);
    }

    return status;
}
#endif

BOOL CParam000Dlg::OnInitDialog()
{
    int i = 0;
    char* wndlst[] = LUAMAN_WNDLIST();
    char* tasks[] = TASK_TYPE_DESC_ARRAY();
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    char buff[128] = {0};
    char* bindHostIdxlst[] = LUAMAN_HOSTLIST();
    
    CDialog::OnInitDialog();

    m_config.taskType = TASK_TYPE_000;

    m_combo_bindHostIdx.Clear();
    for (i = 0; i < sizeof(bindHostIdxlst)/sizeof(bindHostIdxlst[0]); ++i)
    {
        m_combo_bindHostIdx.InsertString(i, bindHostIdxlst[i]);
    }
    m_combo_bindHostIdx.SetCurSel(m_config.curBindHostIdx);
    
    m_combo_WndList.Clear();
    for (i = 0; i < sizeof(wndlst)/sizeof(wndlst[0]); ++i)
    {
        m_combo_WndList.InsertString(i, wndlst[i]);
    }
    m_combo_WndList.SetCurSel(0);

    m_combo_TaskList.Clear();
    for (i = 0; i < sizeof(tasks)/sizeof(tasks[0]); ++i)
    {
        m_combo_TaskList.InsertString(i, tasks[i]);
    }
    m_combo_TaskList.SetCurSel(0);

    m_config.curBindWnd = 0;
    m_config.curBindHostIdx = 0;
    
    //任务列表
    i = 0;
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        m_lstbox_TaskTypeList.InsertString(i, cpos->copyName);
        i++;
    }

    (void)_snprintf(buff, sizeof(buff), "%d", m_config.mixLoopNum);
    m_edit_TaskLoopNum.SetWindowText(buff);

    m_btn_waitEmail.SetCheck(1);
    
    return TRUE;
}

void CParam000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParam000Dlg)
	DDX_Control(pDX, IDC_COMBO_bind_host_idx, m_combo_bindHostIdx);
	DDX_Control(pDX, IDC_CHECK_WAIT_EMAIL, m_btn_waitEmail);
	DDX_Control(pDX, IDC_LIST_task_type, m_lstbox_TaskTypeList);
	DDX_Control(pDX, IDC_EDIT_task_loop_num, m_edit_TaskLoopNum);
	DDX_Control(pDX, IDC_COMBO_bind_wnd, m_combo_WndList);
	DDX_Control(pDX, IDC_COMBO_task_type, m_combo_TaskList);
	DDX_Control(pDX, IDC_BUTTON_task_type_up, m_btn_UpTask);
	DDX_Control(pDX, IDC_BUTTON_task_type_dn, m_btn_DownTask);
	DDX_Control(pDX, IDC_BUTTON_set_task_conf, m_btn_SetTaskConf);
	DDX_Control(pDX, IDC_BUTTON_save_conf, m_btn_SaveConf);
	DDX_Control(pDX, IDC_BUTTON_del_task_type, m_btn_DelTaskType);
	DDX_Control(pDX, IDC_BUTTON_add_task_type, m_btn_AddTaskType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParam000Dlg, CDialog)
	//{{AFX_MSG_MAP(CParam000Dlg)
	ON_BN_CLICKED(IDC_BUTTON_add_task_type, OnBUTTONaddtasktype)
	ON_BN_CLICKED(IDC_BUTTON_del_task_type, OnBUTTONdeltasktype)
	ON_BN_CLICKED(IDC_BUTTON_set_task_conf, OnBUTTONsettaskconf)
	ON_BN_CLICKED(IDC_BUTTON_save_conf, OnBUTTONsaveconf)
	ON_BN_CLICKED(IDC_BUTTON_task_type_up, OnBUTTONtasktypeup)
	ON_BN_CLICKED(IDC_BUTTON_task_type_dn, OnBUTTONtasktypedn)
	ON_CBN_SELCHANGE(IDC_COMBO_bind_wnd, OnSelchangeCOMBObindwnd)
	ON_BN_CLICKED(IDC_CHECK_WAIT_EMAIL, OnCheckWaitEmail)
	ON_BN_CLICKED(IDC_BUTTON_import_accounts, OnBUTTONimportaccounts)
	ON_CBN_SELCHANGE(IDC_COMBO_bind_host_idx, OnSelchangeCOMBObindhostidx)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParam000Dlg message handlers

void CParam000Dlg::OnBUTTONaddtasktype() 
{
	// TODO: Add your control notification handler code here
    char* task[] = TASK_TYPE_DESC_ARRAY();
    int index = m_combo_TaskList.GetCurSel();
    int rndex = 0;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *config = NULL, *cnode = NULL;
    int status = 0;
    int update_flag = 0;
    int mix_task[] = MIX_TASKS();
    int i = 0;
    char buff[64] = {0};
    
    MIX_PARAMS_DIALOG_DECLARE();

    CParameters* params[] = MIX_PARAMS_ARRAY();
    CDialog* param_dlgs[] = MIX_PARAMS_DIALOG_ARRAY();
    
    if (index == TASK_TYPE_000)
    {
        AfxMessageBox("000组合副本不能包含000副本");
        return;
    }
    
    rndex = m_lstbox_TaskTypeList.FindString(0, CString(task[index]));
    if (LB_ERR != rndex)
    {
        AfxMessageBox("副本任务已存在");
        return;
    }

    if (NULL == params[index])
    {
        AfxMessageBox("任务类型不可用");
        return;
    }

    //添加对应类型的配置
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        if (index == cpos->taskType)
        {
            config = cpos;
            break;
        }
    }

    //如果有配置则显示初始值,没有则显示系统默认的值
    if (NULL != config)
    {
        status = params[index]->SetConfig(config);
        if (ERRNO_SUCCESS != status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
    }

    if (TASK_TYPE_SIGNIN != index)
    {
        if (IDOK != param_dlgs[index]->DoModal())
        {
            return;
        }
    }
    
    //更新配置
    config = params[index]->GetConfig();

    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    config->loop_num = atoi(buff);
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = atoi(buff);
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    config->waitEmail = m_config.waitEmail;
    config->curBindWnd = m_combo_WndList.GetCurSel();
    config->curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    //__api_log_MsgInfo("+++++++++ addtastype:%d +++++++++", index);
    //api_PrintConfig(config);
        
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        if (index == cpos->taskType)
        {
            status = api_CloneConfig(cpos, config);
            if (ERRNO_SUCCESS != status)
            {
                AfxMessageBox("新增配置失败");
                return;
            }

            update_flag = 1;
            break;
        }
    }

    //配置文件中没有记录,则新增配置项
    if (!update_flag)
    {
        cnode = (struct cluamanconfig*)LUAMAN_ALLOC(sizeof(struct cluamanconfig));
        if (NULL == cnode)
        {
            AfxMessageBox("内存不足,新增配置失败");
            return;
        }
        LUAMAN_MEMSET(cnode, 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&cnode->node);
        INIT_LIST_HEAD(&cnode->mixConfigList);
        INIT_LIST_HEAD(&cnode->bindinfoList);

        status = api_CloneConfig(cnode, config);
        if (ERRNO_SUCCESS != status)
        {
            //如果是组合副本,需要清空各个任务配置
            for (i = 0; i < LUAMAN_DIM(mix_task); i++)
            {
                if (mix_task[i] != cnode->taskType)
                {
                    continue;
                }
                
                api_ClearMixConfig(cnode);
            }
            
            LUAMAN_FREE(cnode);
            AfxMessageBox("新增配置失败");
            return;
        }

        list_add_tail(&cnode->node, &m_config.mixConfigList);
        m_config.mixConfigNum++;

        api_log_MsgDebug("add mixconfig success, mixConfigNum:%d", m_config.mixConfigNum);
    }
    
    m_lstbox_TaskTypeList.InsertString(m_lstbox_TaskTypeList.GetCount(), task[index]);
    return;
}

void CParam000Dlg::OnBUTTONdeltasktype() 
{
	// TODO: Add your control notification handler code here
    CArray<int,int> aryListBoxSel;
    int nCount = m_lstbox_TaskTypeList.GetSelCount();
    int i = 0;
    int del_index = 0;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *config = NULL;
    CString task_type;
    char* task[] = TASK_TYPE_DESC_ARRAY();
    TASK_TYPE_E taskType = TASK_TYPE_UNKNOWN;
    
    if (nCount <= 0)
    {
        AfxMessageBox("请选择需要移除的任务类型");
        return;
    }

    aryListBoxSel.SetSize(nCount);
    m_lstbox_TaskTypeList.GetSelItems(nCount, aryListBoxSel.GetData()); 

    //从后往前删除
    for (i = nCount - 1; i >= 0; i--)
    {
        del_index = aryListBoxSel.ElementAt(i);
        m_lstbox_TaskTypeList.GetText(del_index, task_type);
        m_lstbox_TaskTypeList.DeleteString(del_index);

        config = NULL;
        taskType = TASK_TYPE_UNKNOWN;
        
        //删除对应的副本配置
        list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
        {
            if (task_type != CString(task[cpos->taskType]))
            {
                continue;
            }

            taskType = cpos->taskType;
            config = cpos;
            break;
        }

        if (NULL == config)
        {
            continue;
        }

        list_del(&config->node);
        m_config.mixConfigNum--;

        api_ClearMixConfig(config);
        LUAMAN_FREE(config);
    }

    return;
}

void CParam000Dlg::OnBUTTONsettaskconf() 
{
	// TODO: Add your control notification handler code here
    int status = 0;
    int nCount = m_lstbox_TaskTypeList.GetSelCount();
    CArray<int,int> aryListBoxSel;
    int i = 0;
    int index = 0;
    CString task_type;
    TASK_TYPE_E taskType = TASK_TYPE_UNKNOWN;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *config = NULL;
    char* task[] = TASK_TYPE_DESC_ARRAY();
    MIX_PARAMS_DIALOG_DECLARE();
    CParameters* params[] = MIX_PARAMS_ARRAY();
    CDialog* param_dlgs[] = MIX_PARAMS_DIALOG_ARRAY();
    char buff[64] = {0};
    
    if (nCount < 1)
    {
        AfxMessageBox("请选择一个任务类型进行设置");
        return;
    }
    
    if (nCount > 1)
    {
        AfxMessageBox("只能选择一个任务类型进行设置");
        return;
    }

    aryListBoxSel.SetSize(nCount);
    m_lstbox_TaskTypeList.GetSelItems(nCount, aryListBoxSel.GetData()); 

    for (i = 0; i < nCount; i++)
    {
        index = aryListBoxSel.ElementAt(i);
        m_lstbox_TaskTypeList.GetText(index, task_type);
        
        list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
        {
            if (task_type != CString(task[cpos->taskType]))
            {
                continue;
            }

            taskType = cpos->taskType;
            config = cpos;
            break;
        }

        break;
    }

    if (NULL == config)
    {
        AfxMessageBox("任务配置缺失,不能修改配置");
        return;
    }

    status = params[taskType]->SetConfig(config);
    if (ERRNO_SUCCESS != status)
    {
        AfxMessageBox("初始化配置失败");
        return;
    }
    
    if (IDOK != param_dlgs[taskType]->DoModal())
    {
        return;
    }

    //更新配置
    config = params[taskType]->GetConfig();

    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    config->loop_num = atoi(buff);
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = atoi(buff);
    config->curBindWnd = m_combo_WndList.GetCurSel();
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    config->waitEmail = m_config.waitEmail;
    config->curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        if (taskType == cpos->taskType)
        {
            status = api_CloneConfig(cpos, config);
            if (ERRNO_SUCCESS != status)
            {
                AfxMessageBox("更新配置失败");
                return;
            }
            return;
        }
    }

    AfxMessageBox("配置不存在,修改失败");
    return;
}

void CParam000Dlg::OnBUTTONsaveconf() 
{
	// TODO: Add your control notification handler code here
	//api_log_MsgInfo("OnBUTTONsaveconf()");
    //api_PrintConfig(&m_config);
    char buff[64]= {0};
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    
    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = m_config.mixLoopNum;
    m_config.curBindWnd = m_combo_WndList.GetCurSel();
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    m_config.curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        cpos->loop_num = m_config.loop_num;
        cpos->mixLoopNum = m_config.mixLoopNum;
        cpos->waitEmail = m_config.waitEmail;
    }
    
	api_SaveXML(std::string(CONFIG_XML), &m_config);
}

void CParam000Dlg::OnBUTTONtasktypeup() 
{
	// TODO: Add your control notification handler code here
    CArray<int,int> aryListBoxSel;
    int nCount = m_lstbox_TaskTypeList.GetSelCount();
    int i = 0;
    int index = 0;
    CString val;
    struct cluamanconfig* cpos = NULL, *cn = NULL, *config = NULL;
    struct list_head list;
    CString task_type;
    char* task[] = TASK_TYPE_DESC_ARRAY();
    
    if (nCount <= 0)
    {
        AfxMessageBox("请选择需要上移的任务类型");
        return;
    }

    aryListBoxSel.SetSize(nCount);
    m_lstbox_TaskTypeList.GetSelItems(nCount, aryListBoxSel.GetData()); 

    if (0 == aryListBoxSel.ElementAt(0))
    {
        AfxMessageBox("不能包含第一个任务类型");
        return;
    }

    for (i = 0; i < nCount; i++)
    {
        index = aryListBoxSel.ElementAt(i);

        m_lstbox_TaskTypeList.GetText(index, val);
        m_lstbox_TaskTypeList.DeleteString(index);
        m_lstbox_TaskTypeList.InsertString(index - 1, val);
    }

    api_log_MsgDebug("---------- Up TaskConf -----------");
    
    //调整配置文件的位置
    INIT_LIST_HEAD(&list);
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        list_del(&cpos->node);
        m_config.mixConfigNum--;
        list_add_tail(&cpos->node, &list);
    }

    for (i = 0; i < m_lstbox_TaskTypeList.GetCount(); i++)
    {
        m_lstbox_TaskTypeList.GetText(i, task_type);

        list_for_each_entry_safe(cpos, cn, &list, node, struct cluamanconfig)
        {
            if (task_type != CString(task[cpos->taskType]))
            {
                continue;
            }

            list_del(&cpos->node);
            list_add_tail(&cpos->node, &m_config.mixConfigList);
            m_config.mixConfigNum++;
            break;
        }
    }

    //api_PrintConfig(&m_config);
    
    return;
}

void CParam000Dlg::OnBUTTONtasktypedn() 
{
	// TODO: Add your control notification handler code here
    CArray<int,int> aryListBoxSel;
    int nCount = m_lstbox_TaskTypeList.GetSelCount();
    int i = 0;
    int del_index = 0;
    CString val;
    char* task[] = TASK_TYPE_DESC_ARRAY();
    struct cluamanconfig* cpos = NULL, *cn = NULL, *config = NULL;
    struct list_head list;
    CString task_type;
    
    if (nCount <= 0)
    {
        AfxMessageBox("请选择需要下移的任务类型");
        return;
    }

    aryListBoxSel.SetSize(nCount);
    m_lstbox_TaskTypeList.GetSelItems(nCount, aryListBoxSel.GetData()); 

    if ((m_lstbox_TaskTypeList.GetCount() - 1) == aryListBoxSel.ElementAt(nCount - 1))
    {
        AfxMessageBox("不能包含最后一个任务类型");
        return;
    }
    
    for (i = nCount - 1; i >= 0; i--)
    {
        del_index = aryListBoxSel.ElementAt(i);

        m_lstbox_TaskTypeList.GetText(del_index, val);
        m_lstbox_TaskTypeList.DeleteString(del_index);
        m_lstbox_TaskTypeList.InsertString(del_index + 1, val);
    }

    api_log_MsgDebug("---------- Down TaskConf -----------");
    
    //调整配置文件的位置
    INIT_LIST_HEAD(&list);
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        list_del(&cpos->node);
        m_config.mixConfigNum--;
        list_add_tail(&cpos->node, &list);
    }

    for (i = 0; i < m_lstbox_TaskTypeList.GetCount(); i++)
    {
        m_lstbox_TaskTypeList.GetText(i, task_type);

        list_for_each_entry_safe(cpos, cn, &list, node, struct cluamanconfig)
        {
            if (task_type != CString(task[cpos->taskType]))
            {
                continue;
            }

            list_del(&cpos->node);
            list_add_tail(&cpos->node, &m_config.mixConfigList);
            m_config.mixConfigNum++;
            break;
        }
    }

    //api_PrintConfig(&m_config);
    
    return;
}

void CParam000Dlg::OnSelchangeCOMBObindwnd() 
{
	// TODO: Add your control notification handler code here
	int index = m_combo_WndList.GetCurSel();
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    char buff[64] = {0};

    //api_log_MsgInfo("index:%d, m_config.curBindWnd:%d", index, m_config.curBindWnd);
    
    if (index != m_config.curBindWnd)
    {
        m_config.curBindWnd = index;
    }

    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = m_config.mixLoopNum;
    m_config.curBindWnd = m_combo_WndList.GetCurSel();
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    m_config.curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        cpos->loop_num = m_config.loop_num;
        cpos->mixLoopNum = m_config.mixLoopNum;
        cpos->waitEmail = m_config.waitEmail;
        cpos->curBindWnd = m_config.curBindWnd;
        cpos->curBindHostIdx = m_config.curBindHostIdx;
    }
    
    return;
}

void CParam000Dlg::OnCheckWaitEmail() 
{
	// TODO: Add your control notification handler code here
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
}

void CParam000Dlg::OnBUTTONimportaccounts() 
{
	// TODO: Add your control notification handler code here
    int index = m_combo_WndList.GetCurSel();
    CString path;
    char* cpath = NULL;
    FILE* fp = NULL;
    char buff[1024] = {0};
    int len = 0;
    int linenum = 0;
    char seps[]   = " ,\t\n";
    char* token = NULL;
    int i = 0;
    struct cluamanaccountinfo* node = NULL, *apos = NULL, *an = NULL;
    struct cluamanbindinfo bindinfo;
    struct ai
    {
        int max_size;
        char* ptr;
    }meta[3] =
    {
        {USRNAME_SIZE, NULL},
        {PASSWORD_SIZE, NULL},
        {PASSWORD_SIZE, NULL},
    };
    int status = 0;
    char curDirectory[512] = {0};
    
    LUAMAN_MEMSET(&bindinfo, 0, sizeof(struct cluamanbindinfo));
    INIT_LIST_HEAD(&bindinfo.node);
    INIT_LIST_HEAD(&bindinfo.accountList);

    bindinfo.bindWndnr = index;

    len = GetCurrentDirectory(sizeof(curDirectory), curDirectory);
    if (len < 1)
    {
        AfxMessageBox("获取当前目录失败");
        return;
    }

    api_log_MsgInfo("导入:当前:目录:%s", curDirectory);
    
    CFileDialog fdlg(TRUE, NULL,
        NULL,
        OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
        TEXT("用户配置文件(*.txt)|*.txt|所有文件(*.*)|*.*|"),
        NULL);
    if(fdlg.DoModal() != IDOK)
    {
        return;
    }

    path = fdlg.GetPathName();
    cpath = path.GetBuffer(BUFFER_MAX);

    api_log_MsgInfo("准备:导入:配置:文件:%s:窗口:%d", cpath, bindinfo.bindWndnr);

    fp = fopen(cpath, "r");
    path.ReleaseBuffer();
    
    if (NULL == fp)
    {
        AfxMessageBox("打开文件失败");
        return;
    }

    LUAMAN_MEMSET(buff, 0, sizeof(buff));
    while (fgets(buff, sizeof(buff) - 1, fp) != NULL)
    {
        len = strlen(buff);
        if (len < 1)
        {
            linenum++;
            LUAMAN_MEMSET(buff, 0, sizeof(buff) - 1);
            continue;
        }

        while (len >= 1 && ('\r' == buff[len - 1] || '\n' == buff[len - 1] || ' ' == buff[len - 1]))
        {
            buff[len - 1] = '\0';
            len--;
        }

        if (len < 1)
        {
            linenum++;
            LUAMAN_MEMSET(buff, 0, sizeof(buff) - 1);
            continue;
        }
        
        api_log_MsgInfo("%s", buff);

        node = (struct cluamanaccountinfo*)LUAMAN_ALLOC(sizeof(struct cluamanaccountinfo));
        if (NULL == node)
        {
            AfxMessageBox("内存不足,导入用户数据失败");
            api_log_MsgError("import accounts info failed, memory short");
            goto finish;
        }

        LUAMAN_MEMSET(node, 0, sizeof(struct cluamanaccountinfo));
        INIT_LIST_HEAD(&node->node);
        meta[0].ptr = node->user_name;
        meta[1].ptr = node->passwd;
        meta[2].ptr = node->passwdex;
        
        i = 0;
        
        token = strtok(buff, seps);
        while(token != NULL)
        {
            if (i >= 3)
            {
                break;
            }

            len = strlen(token) + 1;
            if (len > meta[i].max_size)
            {
                AfxMessageBox("用户数据长度超过限制,导入失败");
                api_log_MsgError("accounts invalid, token:%s, len:%d, linenum:%d",
                    token, len, linenum);
                goto finish;
            }
            
            LUAMAN_MEMCPY(meta[i].ptr, token, strlen(token) + 1);
            token = strtok(NULL, seps);
            i++;
        }

        list_add_tail(&node->node, &bindinfo.accountList);
        bindinfo.accountNum++;
        
        LUAMAN_MEMSET(buff, 0, sizeof(buff) - 1);
        linenum++;
    }

    if (bindinfo.accountNum < 1)
    {
        AfxMessageBox("无有效用户数据,导入失败");
        goto finish;
    }
    
    //拷贝到配置
    status = api_UpdateBindinfo(&m_config, &bindinfo);
    if (ERRNO_SUCCESS != status)
    {
        AfxMessageBox("导入失败");
        goto finish;
    }

    api_log_MsgDebug("导入:数据:成功, bindWndnr:%d, accountNum:%d, linenum:%d",
        bindinfo.bindWndnr, bindinfo.accountNum, linenum);
    //api_PrintConfig(&m_config);
    
    AfxMessageBox("导入成功");
    
finish:
    list_for_each_entry_safe(apos, an, &bindinfo.accountList, node, struct cluamanaccountinfo)
    {
        bindinfo.accountNum--;
        list_del(&apos->node);
        LUAMAN_FREE(apos);
    }

    LUAMAN_FCLOSE(fp);

    if (!SetCurrentDirectory(curDirectory))
    {
        AfxMessageBox("恢复工作目录失败,请重启程序");
        return;
    }

#ifdef LUAMAN_DEBUG
    len = GetCurrentDirectory(sizeof(curDirectory), curDirectory);
    if (len < 1)
    {
        AfxMessageBox("获取当前目录失败");
        return;
    }

    api_log_MsgInfo("恢复:当前:目录:%s", curDirectory);
#endif

    return;
}

void CParam000Dlg::OnSelchangeCOMBObindhostidx() 
{
	// TODO: Add your control notification handler code here
	int index = m_combo_bindHostIdx.GetCurSel();
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    char buff[64] = {0};

    //api_log_MsgInfo("index:%d, m_config.curBindWnd:%d", index, m_config.curBindWnd);
    
    if (index != m_config.curBindHostIdx)
    {
        m_config.curBindHostIdx = index;
    }

    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = m_config.mixLoopNum;
    m_config.curBindWnd = m_combo_WndList.GetCurSel();
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    m_config.curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        cpos->loop_num = m_config.loop_num;
        cpos->mixLoopNum = m_config.mixLoopNum;
        cpos->waitEmail = m_config.waitEmail;
        cpos->curBindWnd = m_config.curBindWnd;
        cpos->curBindHostIdx = m_config.curBindHostIdx;
    }
    
    return;
}

void CParam000Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    struct cluamanconfig* cpos = NULL, *cn = NULL, *cnode = NULL;
    char buff[64] = {0};

    memset(buff, 0, sizeof(buff));
    m_edit_TaskLoopNum.GetWindowText(buff, sizeof(buff));
    m_config.mixLoopNum = atoi(buff);
    m_config.loop_num = m_config.mixLoopNum;
    m_config.curBindWnd = m_combo_WndList.GetCurSel();
    m_config.waitEmail = m_btn_waitEmail.GetCheck();
    m_config.curBindHostIdx = m_combo_bindHostIdx.GetCurSel();
    
    list_for_each_entry_safe(cpos, cn, &m_config.mixConfigList, node, struct cluamanconfig)
    {
        cpos->loop_num = m_config.loop_num;
        cpos->mixLoopNum = m_config.mixLoopNum;
        cpos->waitEmail = m_config.waitEmail;
        cpos->curBindWnd = m_config.curBindWnd;
        cpos->curBindHostIdx = m_config.curBindHostIdx;
    }
    
	CDialog::OnClose();
}
