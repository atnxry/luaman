#if !defined(AFX_PARAMPIRATEDLG_H__20AFD1CF_D59E_4FFC_9F62_9C47DB4FA6DB__INCLUDED_)
#define AFX_PARAMPIRATEDLG_H__20AFD1CF_D59E_4FFC_9F62_9C47DB4FA6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamPirateDlg.h : header file
//

#include "config.h"
#include "param.h"

/////////////////////////////////////////////////////////////////////////////
// ParamPirateDlg dialog

class ParamPirateDlg : public CDialog, public CParameters
{
// Construction
public:
#ifdef HAVE_PARAM_PIRATE_REIMPL
    //struct cluamanconfig m_config;
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
#endif

public:
	ParamPirateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ParamPirateDlg)
	enum { IDD = IDD_DIALOG_param_expirate };
    CEdit	m_edit_task_loop_num;
	CEdit	m_edit_alarm_server;
	CEdit	m_edit_alarm_port;
	CComboBox	m_combo_role;
	CComboBox	m_combo_group;
	CComboBox	m_combo_direction;
	CComboBox	m_combo_blue_threshold;
	CComboBox	m_combo_blue_min;
	CComboBox	m_combo_blue_max;
	CComboBox	m_combo_alarm_host;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ParamPirateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ParamPirateDlg)
	virtual void OnOK();
	virtual void OnCancel();
    virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONalarmtest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMPIRATEDLG_H__20AFD1CF_D59E_4FFC_9F62_9C47DB4FA6DB__INCLUDED_)
