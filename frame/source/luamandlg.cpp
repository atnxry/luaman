// luamanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "luaman.h"
#include "luamanDlg.h"
#include "xrrno.h"
#include "xlog.h"
#include "config.h"
#include "param175dlg.h"
#include "paramfiredlg.h"
#include "param195dlg.h"
#include "param021dlg.h"
#include "param1853dlg.h"
#include "param000dlg.h"
#include "ParamSigninDialog.h"
#include "paramSakuradlg.h"
#include "paramDotaskdlg.h"
#include "ParamFightDlg.h"
#include "ParamSignleDlg.h"
#include "ParamExSignleDlg.h"
#include "ParamPirateDlg.h"
#include "ParamExPirateDlg.h"
#include "ParamEx175Dlg.h"
#include "ParamTeamGroupDlg.h"
#include "task.h"
#include "task175.h"
#include "task195.h"
#include "task021.h"
#include "task1853.h"
#include "task000.h"
#include "taskSignin.h"
#include "taskSakura.h"
#include "taskDotask.h"
#include "taskFight.h"
#include "taskSignle.h"
#include "taskExSignle.h"
#include "taskPirate.h"
#include "taskExPirate.h"
#include "ParamRefugeboatDlg.h"
#include "taskEx175.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuamanDlg dialog

CLuamanDlg::CLuamanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLuamanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLuamanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLuamanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLuamanDlg)
	DDX_Control(pDX, IDC_CHECK_LOG_DEBUG, m_btn_logDebug);
	DDX_Control(pDX, ID_exitEnv, m_btn_exitEnv);
	DDX_Control(pDX, ID_initEnv, m_btn_initEnv);
	DDX_Control(pDX, IDC_COMBO_taskType, m_cb_taskType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLuamanDlg, CDialog)
	//{{AFX_MSG_MAP(CLuamanDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_initEnv, OninitEnv)
	ON_BN_CLICKED(ID_exitEnv, OnexitEnv)
	ON_BN_CLICKED(ID_setConfig, OnsetConfig)
	ON_BN_CLICKED(ID_getConfig, OngetConfig)
	ON_CBN_SELCHANGE(IDC_COMBO_taskType, OnSelchangeCOMBOtaskType)
	ON_BN_CLICKED(IDC_CHECK_LOG_DEBUG, OnCheckLogDebug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuamanDlg message handlers
static struct cluamanoperations g_cluamanops[] = LUAMAN_TASK_OPERATIONS();

#define LUAMAN_OPS_SIZE()   (sizeof(g_cluamanops)/sizeof(g_cluamanops[0]))
#define LUAMAN_OPS_FIRST()  (&g_cluamanops[0])

int CLuamanDlg::init_hmi()
{
    char* tasktype[] = TASK_TYPE_DESC_ARRAY();
    int i = 0;
    int j = 0;
    struct cluamanoperations* ops = NULL;

    for (i = 0; i < sizeof(tasktype)/sizeof(tasktype[0]); i++)
    {
        //api_log_MsgDebug("tasktype[%d]:%s", i, tasktype[i]);
        
        m_cb_taskType.InsertString(i, tasktype[i]);

        LUAMAN_MEMSET(&hmiconfigs[i], 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&hmiconfigs[i].node);
        hmiconfigs[i].loop_num = 0;
        hmiconfigs[i].taskType = (TASK_TYPE_E)i;
        hmiconfigs[i].direction = DIR_LEFT;
        hmiconfigs[i].role = ROLE_M;
        hmiconfigs[i].groupid = 0;
        hmiconfigs[i].maxBlueFiled = 4;
        hmiconfigs[i].minBlue = 4;
        hmiconfigs[i].blueThreshold = 3000;
        LUAMAN_MEMCPY(hmiconfigs[i].alarmServer, LUAMAN_ALARM_SERVER, strlen(LUAMAN_ALARM_SERVER) + 1);
        hmiconfigs[i].alarmPort = 9191;
        hmiconfigs[i].alarmHostIndex = 0;
        hmiconfigs[i].loop_num  = (unsigned int)-1;

        hmiconfigs[i].playWndIndexVal = 0;
        hmiconfigs[i].Wait2Sleep = 0;
        hmiconfigs[i].exitGameCopyFull = 1;
        hmiconfigs[i].cleanPackageWhenStart = 1;
        hmiconfigs[i].callbabydie = 0;

        hmiconfigs[i].copyrolenr = 0;
        
        //新增
        hmiconfigs[i].bluebloodChecktime = 10;
        hmiconfigs[i].blockTimes = 30;
        hmiconfigs[i].waitCopytime = 27;
        hmiconfigs[i].cleanPackageTime = 3;
        CLEAN_PACKAGE_ALL(hmiconfigs[i].cleanPackageX);

        //////////////////////////////////////////////////////////////////////
        //组合任务
        memcpy(hmiconfigs[i].copyName, tasktype[i], strlen(tasktype[i]) + 1);
        INIT_LIST_HEAD(&hmiconfigs[i].mixConfigList);
        hmiconfigs[i].mixConfigNum = 0;
        INIT_LIST_HEAD(&hmiconfigs[i].bindinfoList);
        hmiconfigs[i].bindinfoNum = 0;
        hmiconfigs[i].mixLoopNum = 0;

        api_LoadXML(std::string(CONFIG_XML), tasktype[hmiconfigs[i].taskType], &hmiconfigs[i]);
        
        ops = LUAMAN_OPS_FIRST();
        for (j = 0; j < LUAMAN_OPS_SIZE(); j++)
        {
            if (ops->type == hmiconfigs[i].taskType)
            {
                hmiconfigs[i].ops.type = hmiconfigs[i].taskType;
                hmiconfigs[i].ops.init = ops->init;
                hmiconfigs[i].ops.main = ops->main;
                hmiconfigs[i].ops.exit = ops->exit;

                api_log_MsgInfo("init ops, type:%d", ops->type);
                break;
            }

            ops++;
        }
        
    }
    
    m_cb_taskType.SetCurSel(4);
    configstyle = 0;
    
    m_btn_initEnv.EnableWindow(TRUE);
    m_btn_exitEnv.EnableWindow(FALSE);

    api_log_MsgDebug("init human-machine interface success, tasktype:%d", m_cb_taskType.GetCurSel());
    return ERRNO_SUCCESS;
}

int CLuamanDlg::init_dm_plugin()
{
    keyboardMonitor = NULL;
    
#ifdef HAVE_INIT_DM_PLUGIN
    HRESULT hr = ::CoInitialize(NULL);//初始化线程com库

    if (S_FALSE == hr)
    {
        api_log_MsgWarn("dmsoft has aready been initialized");
        return ERRNO_SUCCESS;
    }

    if (S_OK != hr)
    {
        api_log_MsgError("init dmsoft failed, hr:%ld", hr);
        return ERRNO_FAILED;
    }
    
    dmobj.CreateDispatch("dm.dmsoft");
    
    api_log_MsgDebug("init dmsoft success");
#endif
    
    return ERRNO_SUCCESS;
}

int CLuamanDlg::init_module()
{
    int status = 0;
    char pwd[256] = {0};
    
    status = GetCurrentDirectory(256, pwd);
    RETURN_IF_CONDITION_TRUE((!status), ERRNO_EXCEPTION);
    
    status = api_log_Init(pwd);
    RETURN_IF_CONDITION_TRUE((ERRNO_SUCCESS != status), status);

    status = init_hmi();
    RETURN_IF_CONDITION_TRUE((ERRNO_SUCCESS != status), status);

    status = init_dm_plugin();
    RETURN_IF_CONDITION_TRUE((ERRNO_SUCCESS != status), status);
    
    api_log_MsgDebug("init module success, sizeof(time_t):%d", sizeof(time_t));
    return ERRNO_SUCCESS;
}

BOOL CLuamanDlg::OnInitDialog()
{
    int status = 0;
    
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	status = init_module();
    RETURN_IF_CONDITION_TRUE((ERRNO_SUCCESS != status), FALSE);

    //置顶窗口
    ::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION );
    
	SetWindowText(CString(LUAMAN_TITLE()));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLuamanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLuamanDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLuamanDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLuamanDlg::OninitEnv() 
{
    int status = 0;
    
	// TODO: Add your control notification handler code here
	/* 启动按键监控主线程 */
    if (NULL != keyboardMonitor)
    {
        AfxMessageBox(_T("程序已启动"));
        return;
    }

    keyboardMonitor = new CKeyboardMonitor();
    keyboardMonitor->set(keyboard_monitor_callback, NULL);

    status = keyboardMonitor->run();
    if (status)
    {
        delete keyboardMonitor;
        keyboardMonitor = NULL;

        AfxMessageBox(_T("程序运行错误"));
        api_log_MsgInfo("run keyboard_monitor failed, status:%d", status);
        return;
    }

    m_btn_initEnv.EnableWindow(FALSE);
    m_btn_exitEnv.EnableWindow(TRUE);
    api_log_MsgInfo("run keyboard_monitor success");
    return;
}

void CLuamanDlg::OnexitEnv() 
{
	// TODO: Add your control notification handler code here
	if (NULL == keyboardMonitor)
    {
        AfxMessageBox(_T("程序未启动"));
        return;
    }

    keyboardMonitor->kill();
    delete keyboardMonitor;
    keyboardMonitor = NULL;

    m_btn_initEnv.EnableWindow(TRUE);
    m_btn_exitEnv.EnableWindow(FALSE);
    
    return;
}

void CLuamanDlg::update_config(void)
{
    int index = m_cb_taskType.GetCurSel();
    struct cluamanconfig* config = &hmiconfigs[index];
    struct cluamanconfig* pos = NULL;
    struct cluamanconfig* n = NULL;
    struct cluamanconfig* node = NULL;
    struct cluamanconfiglist list;
    
    INIT_LIST_HEAD(&list.head);

    switch (index)
    {
    case TASK_TYPE_164:
    case TASK_TYPE_175:
    case TASK_TYPE_1852:
    case TASK_TYPE_1853:
    case TASK_TYPE_195:
    case TASK_TYPE_021:
    case TASK_TYPE_TEAMGROUP:
    case TASK_TYPE_000:
    case TASK_TYPE_SIGNIN:
    case TASK_TYPE_SAKURA:
    case TASK_TYPE_FIGHT:
    case TASK_TYPE_SIGNLE:
    case TASK_TYPE_EXSINGLE:
    case TASK_TYPE_EX175:
    case TASK_TYPE_DOTASK:
    case TASK_TYPE_PIRATE:
    case TASK_TYPE_EXPIRATE:
    case TASK_TYPE_FIRE:
    case TASK_TYPE_SNAPSHOT:
    case TASK_TYPE_SIMPLE_FIGHT:
    case TASK_TYPE_REFUGEBOAT:
        node = (struct cluamanconfig*)LUAMAN_ALLOC(sizeof(struct cluamanconfig));
        if (NULL == node)
        {
            api_log_MsgDebug("update config failed, memory short");
            return;
        }

        LUAMAN_MEMSET(node, 0, sizeof(struct cluamanconfig));
        INIT_LIST_HEAD(&node->node);
        INIT_LIST_HEAD(&node->mixConfigList);
        INIT_LIST_HEAD(&node->bindinfoList);
        
        api_UpdateConfig(node, config, 1);

        list_add_tail(&node->node, &list.head);
        list.count = 1;
        break;

    default:
        {
            api_log_MsgDebug("update config: it's not possbile to reach here");
            return;
        }
        break;
    }
    
    g_luamantaskmgr.setCurConfig(&list);
    
    //api_log_MsgDebug("update config finish, tasktype:%d", index);
    return;
}

void CLuamanDlg::OnsetConfig() 
{
    int status = 0;
    int index = m_cb_taskType.GetCurSel();
    struct cluamanconfig* oconfig = &hmiconfigs[index];
    struct cluamanconfig* nconfig = NULL;
    CParam175Dlg dlg175;
    CParamFireDlg dlgfirm;
    CParam195Dlg dlg195;
    CParam021Dlg dlg021;
    CParam1853Dlg dlg1853;
    CParam000Dlg dlg000;
    CParamSigninDialog dlgsignin;
    CParamSakuraDlg dlgsakura;
    CParamDotaskDlg dlgdotask;
    CParamFightDlg dlgfight;
    CParamSignleDlg dlgsignle;
    ParamPirateDlg dlgpirate;
    ParamExPirateDlg dlgexpirate;
    CParamRefugeboatDlg dlgrefugeboat;
    CParamExsignleDlg dlgexsingle;
    CParamEx175Dlg dlgex175;
    CParamTeamGroupDlg dlgteamgroup;
    
    switch (index)
    {
    case TASK_TYPE_TEAMGROUP:
        status = dlgteamgroup.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }

        dlgteamgroup.SetSelectedFrontIndex(oconfig->selected_front_index);
        dlgteamgroup.SetDirection(oconfig->direction);
        dlgteamgroup.SetRole(oconfig->role);
        dlgteamgroup.SetGroup(oconfig->groupid);
        dlgteamgroup.SetBlueMax(oconfig->maxBlueFiled);
        dlgteamgroup.SetBlueMin(oconfig->minBlue);
        dlgteamgroup.SetBlueThreshold(oconfig->blueThreshold);
        dlgteamgroup.SetServer(oconfig->alarmServer);
        dlgteamgroup.SetPort(oconfig->alarmPort);
        dlgteamgroup.SetHost(oconfig->alarmHostIndex);
        dlgteamgroup.SetLoopNum(oconfig->loop_num);
        dlgteamgroup.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgteamgroup.SetTransWait(oconfig->Wait2Sleep);
        dlgteamgroup.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgteamgroup.SetExitGameCopyFull(oconfig->exitGameCopyFull);
        
        if (IDOK != dlgteamgroup.DoModal())
        {
            return;
        }
        
        nconfig = dlgteamgroup.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;
    
    case TASK_TYPE_REFUGEBOAT:
        status = dlgrefugeboat.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlgrefugeboat.SetDirection(oconfig->direction);
        dlgrefugeboat.SetRole(oconfig->role);
        dlgrefugeboat.SetGroup(oconfig->groupid);
        dlgrefugeboat.SetBlueMax(oconfig->maxBlueFiled);
        dlgrefugeboat.SetBlueMin(oconfig->minBlue);
        dlgrefugeboat.SetBlueThreshold(oconfig->blueThreshold);
        dlgrefugeboat.SetServer(oconfig->alarmServer);
        dlgrefugeboat.SetPort(oconfig->alarmPort);
        dlgrefugeboat.SetHost(oconfig->alarmHostIndex);
        dlgrefugeboat.SetLoopNum(oconfig->loop_num);
        dlgrefugeboat.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgrefugeboat.SetTransWait(oconfig->Wait2Sleep);
        dlgrefugeboat.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgrefugeboat.SetCallBabyDie(oconfig->callbabydie);
        dlgrefugeboat.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlgrefugeboat.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlgrefugeboat.SetCleanPackage(oconfig->cleanPackageX);
        dlgrefugeboat.SetWaitCopytime(oconfig->waitCopytime);
        dlgrefugeboat.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlgrefugeboat.SetBlockTimes(oconfig->blockTimes);
        
        if (IDOK != dlgrefugeboat.DoModal())
        {
            return;
        }
        
        nconfig = dlgrefugeboat.GetConfig();
        
        //__api_log_MsgDebug("<<<<<<<<<<<< set param1853");
        //api_PrintConfig(nconfig);
        
        api_UpdateConfig(oconfig, nconfig, 0);

        //__api_log_MsgDebug(">>>>>>>>>>>> set param1853 finish");
        //api_PrintConfig(oconfig);
        
        break;
        
    case TASK_TYPE_FIGHT:
        status = dlgfight.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgfight.DoModal())
        {
            return;
        }

        nconfig = dlgfight.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;
        
    case TASK_TYPE_DOTASK:
        status = dlgdotask.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgdotask.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgdotask.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_PIRATE:
        status = dlgpirate.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgpirate.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgpirate.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_EXPIRATE:
        status = dlgexpirate.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgexpirate.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgexpirate.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_SIGNLE:
        status = dlgsignle.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgsignle.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgsignle.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_EXSINGLE:
        status = dlgexsingle.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgexsingle.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgexsingle.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_EX175:
        status = dlgex175.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgex175.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgex175.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;
        
    case TASK_TYPE_SAKURA:
        status = dlgsakura.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgsakura.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgsakura.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;
        
    case TASK_TYPE_SIGNIN:
        status = dlgsignin.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlgsignin.DoModal())
        {
            return;//没有取消按钮
        }

        nconfig = dlgsignin.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;
        
    case TASK_TYPE_000:
        status = dlg000.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        if (IDOK != dlg000.DoModal())
        {
            //return;//没有取消按钮
        }

        nconfig = dlg000.GetConfig();

        //api_log_MsgInfo("-------------- setdlg --------------");
        api_UpdateConfig(oconfig, nconfig, 0);

        
        //api_PrintConfig(nconfig);
        
        break;
        
    case TASK_TYPE_195:
        status = dlg195.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlg195.SetDirection(oconfig->direction);
        dlg195.SetRole(oconfig->role);
        dlg195.SetGroup(oconfig->groupid);
        dlg195.SetBlueMax(oconfig->maxBlueFiled);
        dlg195.SetBlueMin(oconfig->minBlue);
        dlg195.SetBlueThreshold(oconfig->blueThreshold);
        dlg195.SetServer(oconfig->alarmServer);
        dlg195.SetPort(oconfig->alarmPort);
        dlg195.SetHost(oconfig->alarmHostIndex);
        dlg195.SetLoopNum(oconfig->loop_num);
        
        if (IDOK != dlg195.DoModal())
        {
            return;
        }
        
        nconfig = dlg195.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        
        break;

    case TASK_TYPE_021:
        status = dlg021.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlg021.SetDirection(oconfig->direction);
        dlg021.SetRole(oconfig->role);
        dlg021.SetGroup(oconfig->groupid);
        dlg021.SetBlueMax(oconfig->maxBlueFiled);
        dlg021.SetBlueMin(oconfig->minBlue);
        dlg021.SetBlueThreshold(oconfig->blueThreshold);
        dlg021.SetServer(oconfig->alarmServer);
        dlg021.SetPort(oconfig->alarmPort);
        dlg021.SetHost(oconfig->alarmHostIndex);
        dlg021.SetLoopNum(oconfig->loop_num);
        dlg021.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg021.SetTransWait(oconfig->Wait2Sleep);
        dlg021.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg021.SetExitGameCopyFull(oconfig->exitGameCopyFull);
        
        if (IDOK != dlg021.DoModal())
        {
            return;
        }
        
        nconfig = dlg021.GetConfig();
        api_UpdateConfig(oconfig, nconfig, 0);
        break;

    case TASK_TYPE_1853:
        status = dlg1853.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlg1853.SetDirection(oconfig->direction);
        dlg1853.SetRole(oconfig->role);
        dlg1853.SetGroup(oconfig->groupid);
        dlg1853.SetBlueMax(oconfig->maxBlueFiled);
        dlg1853.SetBlueMin(oconfig->minBlue);
        dlg1853.SetBlueThreshold(oconfig->blueThreshold);
        dlg1853.SetServer(oconfig->alarmServer);
        dlg1853.SetPort(oconfig->alarmPort);
        dlg1853.SetHost(oconfig->alarmHostIndex);
        dlg1853.SetLoopNum(oconfig->loop_num);
        dlg1853.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg1853.SetTransWait(oconfig->Wait2Sleep);
        dlg1853.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg1853.SetCallBabyDie(oconfig->callbabydie);
        dlg1853.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlg1853.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlg1853.SetCleanPackage(oconfig->cleanPackageX);
        dlg1853.SetWaitCopytime(oconfig->waitCopytime);
        dlg1853.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlg1853.SetBlockTimes(oconfig->blockTimes);
        
        if (IDOK != dlg1853.DoModal())
        {
            return;
        }
        
        nconfig = dlg1853.GetConfig();
        
        //__api_log_MsgDebug("<<<<<<<<<<<< set param1853");
        //api_PrintConfig(nconfig);
        
        api_UpdateConfig(oconfig, nconfig, 0);

        //__api_log_MsgDebug(">>>>>>>>>>>> set param1853 finish");
        //api_PrintConfig(oconfig);
        
        break;

    case TASK_TYPE_175:
        status = dlg175.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlg175.SetDirection(oconfig->direction);
        dlg175.SetRole(oconfig->role);
        dlg175.SetGroup(oconfig->groupid);
        dlg175.SetBlueMax(oconfig->maxBlueFiled);
        dlg175.SetBlueMin(oconfig->minBlue);
        dlg175.SetBlueThreshold(oconfig->blueThreshold);
        dlg175.SetServer(oconfig->alarmServer);
        dlg175.SetPort(oconfig->alarmPort);
        dlg175.SetHost(oconfig->alarmHostIndex);
        dlg175.SetLoopNum(oconfig->loop_num);
        dlg175.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg175.SetTransWait(oconfig->Wait2Sleep);
        dlg175.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg175.SetCallBabyDie(oconfig->callbabydie);
        dlg175.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlg175.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlg175.SetCleanPackage(oconfig->cleanPackageX);
        dlg175.SetWaitCopytime(oconfig->waitCopytime);
        dlg175.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlg175.SetBlockTimes(oconfig->blockTimes);
        
        if (IDOK != dlg175.DoModal())
        {
            return;
        }
        
        nconfig = dlg175.GetConfig();
        
        api_UpdateConfig(oconfig, nconfig, 0);

        break;

    case TASK_TYPE_FIRE:
        status = dlgfirm.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        
        dlgfirm.SetDirection(oconfig->direction);
        dlgfirm.SetRole(oconfig->role);
        dlgfirm.SetGroup(oconfig->groupid);
        dlgfirm.SetBlueMax(oconfig->maxBlueFiled);
        dlgfirm.SetBlueMin(oconfig->minBlue);
        dlgfirm.SetBlueThreshold(oconfig->blueThreshold);
        dlgfirm.SetServer(oconfig->alarmServer);
        dlgfirm.SetPort(oconfig->alarmPort);
        dlgfirm.SetHost(oconfig->alarmHostIndex);
        dlgfirm.SetLoopNum(oconfig->loop_num);
        dlgfirm.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgfirm.SetTransWait(oconfig->Wait2Sleep);
        dlgfirm.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgfirm.SetCallBabyDie(oconfig->callbabydie);
        dlgfirm.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlgfirm.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlgfirm.SetCleanPackage(oconfig->cleanPackageX);
        dlgfirm.SetWaitCopytime(oconfig->waitCopytime);
        dlgfirm.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlgfirm.SetBlockTimes(oconfig->blockTimes);
        
        if (IDOK != dlgfirm.DoModal())
        {
            return;
        }
        
        nconfig = dlgfirm.GetConfig();
        
        api_UpdateConfig(oconfig, nconfig, 0);

        break;
        
    default:
        break;
    }

    update_config();
    return;
}

void CLuamanDlg::OngetConfig() 
{
    int index = m_cb_taskType.GetCurSel();
    struct cluamanconfig* oconfig = &hmiconfigs[index];
    int status = 0;
    
	// TODO: Add your control notification handler code here
	CParam195Dlg dlg195;
    CParam021Dlg dlg021;
    CParam1853Dlg dlg1853;
    CParam175Dlg dlg175;
    CParamFireDlg dlgfirm;
    CParam000Dlg dlg000;
    CParamSakuraDlg dlgsakura;
    CParamFightDlg dlgfight;
    CParamRefugeboatDlg dlgrefugeboat;
    CParamTeamGroupDlg dlgteamgroup;
    
    switch (index)
    {
    case TASK_TYPE_TEAMGROUP:
        dlgteamgroup.SetSelectedFrontIndex(oconfig->selected_front_index);
        dlgteamgroup.SetDirection(oconfig->direction);
        dlgteamgroup.SetRole(oconfig->role);
        dlgteamgroup.SetGroup(oconfig->groupid);
        dlgteamgroup.SetBlueMax(oconfig->maxBlueFiled);
        dlgteamgroup.SetBlueMin(oconfig->minBlue);
        dlgteamgroup.SetBlueThreshold(oconfig->blueThreshold);
        dlgteamgroup.SetServer(oconfig->alarmServer);
        dlgteamgroup.SetPort(oconfig->alarmPort);
        dlgteamgroup.SetHost(oconfig->alarmHostIndex);
        dlgteamgroup.SetLoopNum(oconfig->loop_num);
        dlgteamgroup.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgteamgroup.SetTransWait(oconfig->Wait2Sleep);
        dlgteamgroup.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgteamgroup.SetExitGameCopyFull(oconfig->exitGameCopyFull);
        
        dlgteamgroup.DoModal();
        break;
        
    case TASK_TYPE_REFUGEBOAT:
        dlgrefugeboat.SetDirection(oconfig->direction);
        dlgrefugeboat.SetRole(oconfig->role);
        dlgrefugeboat.SetGroup(oconfig->groupid);
        dlgrefugeboat.SetBlueMax(oconfig->maxBlueFiled);
        dlgrefugeboat.SetBlueMin(oconfig->minBlue);
        dlgrefugeboat.SetBlueThreshold(oconfig->blueThreshold);
        dlgrefugeboat.SetServer(oconfig->alarmServer);
        dlgrefugeboat.SetPort(oconfig->alarmPort);
        dlgrefugeboat.SetHost(oconfig->alarmHostIndex);
        dlgrefugeboat.SetLoopNum(oconfig->loop_num);
        dlgrefugeboat.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgrefugeboat.SetTransWait(oconfig->Wait2Sleep);
        dlgrefugeboat.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgrefugeboat.SetCallBabyDie(oconfig->callbabydie);
        dlgrefugeboat.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlgrefugeboat.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlgrefugeboat.SetCleanPackage(oconfig->cleanPackageX);
        dlgrefugeboat.SetWaitCopytime(oconfig->waitCopytime);
        dlgrefugeboat.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlgrefugeboat.SetBlockTimes(oconfig->blockTimes);

        //__api_log_MsgDebug("========= get param1853");
        //api_PrintConfig(oconfig);
        //__api_log_MsgDebug("========= get param1853 finish");
        
        dlgrefugeboat.DoModal();
        break;
        
    case TASK_TYPE_FIGHT:
        status = dlgfight.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        dlgfight.DoModal();
        
        break;
        
    case TASK_TYPE_SAKURA:
        status = dlgsakura.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        dlgsakura.DoModal();
        
        break;
        
    case TASK_TYPE_000:
        status = dlg000.SetConfig(oconfig);
        if (status)
        {
            AfxMessageBox("初始化配置失败");
            return;
        }
        dlg000.DoModal();
        
        break;
        
    case TASK_TYPE_195:
        dlg195.SetDirection(oconfig->direction);
        dlg195.SetRole(oconfig->role);
        dlg195.SetGroup(oconfig->groupid);
        dlg195.SetBlueMax(oconfig->maxBlueFiled);
        dlg195.SetBlueMin(oconfig->minBlue);
        dlg195.SetBlueThreshold(oconfig->blueThreshold);
        dlg195.SetServer(oconfig->alarmServer);
        dlg195.SetPort(oconfig->alarmPort);
        dlg195.SetHost(oconfig->alarmHostIndex);
        dlg195.SetLoopNum(oconfig->loop_num);

        dlg195.DoModal();
        break;

    case TASK_TYPE_021:
        dlg021.SetDirection(oconfig->direction);
        dlg021.SetRole(oconfig->role);
        dlg021.SetGroup(oconfig->groupid);
        dlg021.SetBlueMax(oconfig->maxBlueFiled);
        dlg021.SetBlueMin(oconfig->minBlue);
        dlg021.SetBlueThreshold(oconfig->blueThreshold);
        dlg021.SetServer(oconfig->alarmServer);
        dlg021.SetPort(oconfig->alarmPort);
        dlg021.SetHost(oconfig->alarmHostIndex);
        dlg021.SetLoopNum(oconfig->loop_num);
        dlg021.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg021.SetTransWait(oconfig->Wait2Sleep);
        dlg021.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg021.SetExitGameCopyFull(oconfig->exitGameCopyFull);
        
        dlg021.DoModal();
        break;

    case TASK_TYPE_1853:
        dlg1853.SetDirection(oconfig->direction);
        dlg1853.SetRole(oconfig->role);
        dlg1853.SetGroup(oconfig->groupid);
        dlg1853.SetBlueMax(oconfig->maxBlueFiled);
        dlg1853.SetBlueMin(oconfig->minBlue);
        dlg1853.SetBlueThreshold(oconfig->blueThreshold);
        dlg1853.SetServer(oconfig->alarmServer);
        dlg1853.SetPort(oconfig->alarmPort);
        dlg1853.SetHost(oconfig->alarmHostIndex);
        dlg1853.SetLoopNum(oconfig->loop_num);
        dlg1853.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg1853.SetTransWait(oconfig->Wait2Sleep);
        dlg1853.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg1853.SetCallBabyDie(oconfig->callbabydie);
        dlg1853.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlg1853.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlg1853.SetCleanPackage(oconfig->cleanPackageX);
        dlg1853.SetWaitCopytime(oconfig->waitCopytime);
        dlg1853.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlg1853.SetBlockTimes(oconfig->blockTimes);

        //__api_log_MsgDebug("========= get param1853");
        //api_PrintConfig(oconfig);
        //__api_log_MsgDebug("========= get param1853 finish");
        
        dlg1853.DoModal();
        break;

    case TASK_TYPE_175:
        dlg175.SetDirection(oconfig->direction);
        dlg175.SetRole(oconfig->role);
        dlg175.SetGroup(oconfig->groupid);
        dlg175.SetBlueMax(oconfig->maxBlueFiled);
        dlg175.SetBlueMin(oconfig->minBlue);
        dlg175.SetBlueThreshold(oconfig->blueThreshold);
        dlg175.SetServer(oconfig->alarmServer);
        dlg175.SetPort(oconfig->alarmPort);
        dlg175.SetHost(oconfig->alarmHostIndex);
        dlg175.SetLoopNum(oconfig->loop_num);
        dlg175.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlg175.SetTransWait(oconfig->Wait2Sleep);
        dlg175.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlg175.SetCallBabyDie(oconfig->callbabydie);
        dlg175.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlg175.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlg175.SetCleanPackage(oconfig->cleanPackageX);
        dlg175.SetWaitCopytime(oconfig->waitCopytime);
        dlg175.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlg175.SetBlockTimes(oconfig->blockTimes);

        dlg175.DoModal();
        break;

    case TASK_TYPE_FIRE:
        dlgfirm.SetDirection(oconfig->direction);
        dlgfirm.SetRole(oconfig->role);
        dlgfirm.SetGroup(oconfig->groupid);
        dlgfirm.SetBlueMax(oconfig->maxBlueFiled);
        dlgfirm.SetBlueMin(oconfig->minBlue);
        dlgfirm.SetBlueThreshold(oconfig->blueThreshold);
        dlgfirm.SetServer(oconfig->alarmServer);
        dlgfirm.SetPort(oconfig->alarmPort);
        dlgfirm.SetHost(oconfig->alarmHostIndex);
        dlgfirm.SetLoopNum(oconfig->loop_num);
        dlgfirm.SetPalyWndIndex(oconfig->playWndIndexVal);
        dlgfirm.SetTransWait(oconfig->Wait2Sleep);
        dlgfirm.SetCleanPackageWhenStart(oconfig->cleanPackageWhenStart);
        dlgfirm.SetCallBabyDie(oconfig->callbabydie);
        dlgfirm.SetExitGameCopyFull(oconfig->exitGameCopyFull);

        //新增
        dlgfirm.SetCleanPackageTime(oconfig->cleanPackageTime);
        dlgfirm.SetCleanPackage(oconfig->cleanPackageX);
        dlgfirm.SetWaitCopytime(oconfig->waitCopytime);
        dlgfirm.SetBluebloodChecktime(oconfig->bluebloodChecktime);
        dlgfirm.SetBlockTimes(oconfig->blockTimes);

        dlgfirm.DoModal();
        break;
        
    default:
        break;
    }
    
    return;
}

void CLuamanDlg::OnSelchangeCOMBOtaskType() 
{
	// TODO: Add your control notification handler code here
	if (configstyle == m_cb_taskType.GetCurSel())
    {
        return;
    }

    configstyle = m_cb_taskType.GetCurSel();
    update_config();
    
    return;
}

void CLuamanDlg::OnCheckLogDebug() 
{
	// TODO: Add your control notification handler code here
    int state = m_btn_logDebug.GetCheck();

    if (1 == state || 0 == state)
    {
        api_log_SetLevel(LE_DEBUG, state);
        return;
    }

    return;
}

BOOL CLuamanDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        switch (pMsg->wParam)
        {
        case VK_ESCAPE:
            return TRUE;
        default:
            break;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

