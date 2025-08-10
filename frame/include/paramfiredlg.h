#if !defined(AFX_PARAMFIREDLG_H__1C755C89_DF97_43B2_97A1_D5200C841D55__INCLUDED_)
#define AFX_PARAMFIREDLG_H__1C755C89_DF97_43B2_97A1_D5200C841D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamFireDlg.h : header file
//
#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamFireDlg dialog

class CParamFireDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamFireDlg(CWnd* pParent = NULL);   // standard constructor
#ifdef HAVE_PARAM_FIRE_REIMPL
    //struct cluamanconfig* GetConfig(){return &m_config;}
    void SetDirection(int direction);
    void SetRole(int role);
    void SetGroup(int grp);
    void SetBlueMax(int bluemax);
    void SetBlueMin(int bluemin);
    void SetBlueThreshold(int bluethreshold);
    void SetServer(char* server);
    void SetPort(int port);
    void SetHost(int host);
    void SetLoopNum(unsigned int loopnum);
    void SetPalyWndIndex(unsigned int playWndIndexVal);
    void SetTransWait(int Wait2Sleep);
    void SetExitGameCopyFull(int exitGameCopyFull);

    //ÐÂÔö
    void SetCallBabyDie(int callbabydie);
    void SetCleanPackageTime(int cleanPackageTime);
    void SetCleanPackageWhenStart(int cleanPackageWhenStart);
    void SetCleanPackage1(int cleanPackage1);
    void SetCleanPackage2(int cleanPackage2);
    void SetCleanPackage3(int cleanPackage3);
    void SetCleanPackage4(int cleanPackage4);
    void SetCleanPackage5(int cleanPackage5);
    void SetCleanPackage(int cleanPackage);
    void SetWaitCopytime(int waitCopytime);
    void SetBluebloodChecktime(int bluebloodChecktime);
    void SetBlockTimes(int blockTimes);
#endif

// Dialog Data
	//{{AFX_DATA(CParamFireDlg)
	enum { IDD = IDD_DIALOG_param_fire };
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_waitCopytime;
	CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_cleanPackageTime;
	CComboBox	m_combo_bluebloodChecktime;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_blockTimes;
	CComboBox	m_combo_alarm_host;
	CButton	m_btn_cleanPackageWhenStart;
	CButton	m_btn_cleanPackage5;
	CButton	m_btn_cleanPackage4;
	CButton	m_btn_cleanPackage3;
	CButton	m_btn_cleanPackage2;
	CButton	m_btn_cleanPackage1;
	CButton	m_btn_callBabyDie;
	CButton	m_btn_exit_game;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamFireDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamFireDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMFIREDLG_H__1C755C89_DF97_43B2_97A1_D5200C841D55__INCLUDED_)
