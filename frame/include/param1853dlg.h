#if !defined(AFX_PARAM1853DLG_H__22B650E0_10B7_4E41_9ADB_E482ECF87DD8__INCLUDED_)
#define AFX_PARAM1853DLG_H__22B650E0_10B7_4E41_9ADB_E482ECF87DD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Param1853Dlg.h : header file
//
#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParam1853Dlg dialog

class CParam1853Dlg : public CDialog, public CParameters
{
// Construction
public:
	CParam1853Dlg(CWnd* pParent = NULL);   // standard constructor

public:
    //struct cluamanconfig m_config;
#ifdef HAVE_PARAM1853_REIMPL
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
	//{{AFX_DATA(CParam1853Dlg)
	enum { IDD = IDD_DIALOG_param_1853 };
	CButton	m_btn_callBabyDie;
    CComboBox	m_combo_cleanPackageTime;
	CButton	m_btn_cleanPackageWhenStart;
	CButton	m_btn_cleanPackage5;
	CButton	m_btn_cleanPackage4;
	CButton	m_btn_cleanPackage3;
	CButton	m_btn_cleanPackage1;
    CButton	m_btn_cleanPackage2;
	CComboBox	m_combo_waitCopytime;
	CComboBox	m_combo_bluebloodChecktime;
    CComboBox	m_combo_blockTimes;
    CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_alarm_host;
	CButton	m_btn_exit_game;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParam1853Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParam1853Dlg)
		// NOTE: the ClassWizard will add member functions here
    virtual void OnOK();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAM1853DLG_H__22B650E0_10B7_4E41_9ADB_E482ECF87DD8__INCLUDED_)
