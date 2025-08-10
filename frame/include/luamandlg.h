// luamanDlg.h : header file
//

#if !defined(AFX_LUAMANDLG_H__E0E1115A_23C5_4A23_BB3B_74C2F9DCA5EE__INCLUDED_)
#define AFX_LUAMANDLG_H__E0E1115A_23C5_4A23_BB3B_74C2F9DCA5EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLuamanDlg dialog

#include "keyboard_monitor.h"
#include "config.h"
#include "task.h"

class CLuamanDlg : public CDialog
{
// Construction
public:
	CLuamanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLuamanDlg)
	enum { IDD = IDD_LUAMAN_DIALOG };
	CButton	m_btn_logDebug;
	CButton	m_btn_exitEnv;
	CButton	m_btn_initEnv;
	CComboBox	m_cb_taskType;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuamanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
    int init_hmi();
    int init_module();
    int init_dm_plugin();
    void update_config(void);
    
public:
    Idmsoft dmobj;
    CKeyboardMonitor* keyboardMonitor;
    int configstyle;
    struct cluamanconfig hmiconfigs[TASK_TYPE_NUM];

    BOOL PreTranslateMessage(MSG* pMsg);
    
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLuamanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OninitEnv();
	afx_msg void OnexitEnv();
	afx_msg void OnsetConfig();
	afx_msg void OngetConfig();
	afx_msg void OnSelchangeCOMBOtaskType();
	afx_msg void OnCheckLogDebug();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAMANDLG_H__E0E1115A_23C5_4A23_BB3B_74C2F9DCA5EE__INCLUDED_)
