
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "ListenSock.h"
#include "ClientSock.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	CListenSock* m_pListenSock;
	HBITMAP hbmScreen;

// Operations
public:
	void ProcessPendingAccept();

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

