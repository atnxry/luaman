#if !defined(AFX_PARAMSAKURADLG_H__B8842A9C_10B2_48F6_9C8A_6F9A839FD981__INCLUDED_)
#define AFX_PARAMSAKURADLG_H__B8842A9C_10B2_48F6_9C8A_6F9A839FD981__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamSakuraDlg.h : header file
//
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamSakuraDlg dialog

class CParamSakuraDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamSakuraDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamSakuraDlg)
	enum { IDD = IDD_DIALOG_param_sakura };
	CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_wait2sleep;
	CComboBox	m_combo_role;
	CComboBox	m_combo_playWndIndexVal;
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
	//{{AFX_VIRTUAL(CParamSakuraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamSakuraDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSAKURADLG_H__B8842A9C_10B2_48F6_9C8A_6F9A839FD981__INCLUDED_)
