#if !defined(AFX_PARAMDOTASKDLG_H__B92DC876_D251_46BC_B45A_F9AED9BE94BE__INCLUDED_)
#define AFX_PARAMDOTASKDLG_H__B92DC876_D251_46BC_B45A_F9AED9BE94BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamDotaskDlg.h : header file
//
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamDotaskDlg dialog

class CParamDotaskDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamDotaskDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamDotaskDlg)
	enum { IDD = IDD_DIALOG_param_dotask };
	CComboBox	m_combo_swjdIndex;
	CComboBox	m_combo_buddyName;
	CEdit	m_edit_alarm_port;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_task_loop_num;
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
	//{{AFX_VIRTUAL(CParamDotaskDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamDotaskDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMDOTASKDLG_H__B92DC876_D251_46BC_B45A_F9AED9BE94BE__INCLUDED_)
