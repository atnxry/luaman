#if !defined(AFX_PARAMSIGNLEDLG_H__C84642D5_B38D_4C80_B0EB_26D9B54795BD__INCLUDED_)
#define AFX_PARAMSIGNLEDLG_H__C84642D5_B38D_4C80_B0EB_26D9B54795BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamSignleDlg.h : header file
//
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// CParamSignleDlg dialog

class CParamSignleDlg : public CDialog, public CParameters
{
// Construction
public:
	CParamSignleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamSignleDlg)
	enum { IDD = IDD_DIALOG_param_single };
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
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamSignleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamSignleDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSIGNLEDLG_H__C84642D5_B38D_4C80_B0EB_26D9B54795BD__INCLUDED_)
