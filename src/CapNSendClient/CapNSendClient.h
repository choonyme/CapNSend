
// CapNSendClient.h : main header file for the CapNSendClient application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CCapNSendClientApp:
// See CapNSendClient.cpp for the implementation of this class
//

class CCapNSendClientApp : public CWinAppEx
{
public:
	CCapNSendClientApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCapNSendClientApp theApp;
