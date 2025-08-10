// luaman.h : main header file for the LUAMAN application
//

#if !defined(AFX_LUAMAN_H__6FAFE20A_1F10_4B6E_B565_9D1E508142CC__INCLUDED_)
#define AFX_LUAMAN_H__6FAFE20A_1F10_4B6E_B565_9D1E508142CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLuamanApp:
// See luaman.cpp for the implementation of this class
//
class CLuamanApp : public CWinApp
{
public:
	CLuamanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuamanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLuamanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAMAN_H__6FAFE20A_1F10_4B6E_B565_9D1E508142CC__INCLUDED_)
