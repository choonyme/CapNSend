
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "CapNSendClient.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_pListenSock = NULL;
	m_pListenSock = new CListenSock(this);

	if(m_pListenSock->Create(60601))
	{
		if(!m_pListenSock->Listen()){
			OutputDebugString("Cannot listen to port!");
		}
		//TODO: else exit app with error
	}

	hbmScreen = NULL;
}

CChildView::~CChildView()
{
	delete m_pListenSock;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(hbmScreen != NULL){
		BITMAP bitmap;
		::GetObject(hbmScreen, sizeof(bitmap), &bitmap);

		HDC hdcMem = CreateCompatibleDC(*GetDC());
		if(!hdcMem){
			OutputDebugString("Cannot create compatible DC\n");
			return;
		}

		SelectObject(hdcMem,hbmScreen);
		CRect wndRect;
		GetClientRect(wndRect);
		StretchBlt(dc, 0, 0,  wndRect.Width(), wndRect.Height(),hdcMem, 0, 0, bitmap.bmWidth - 1, bitmap.bmHeight - 1, SRCCOPY);

		DeleteDC(hdcMem);
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CChildView::ProcessPendingAccept(){
	CClientSock* pClientSock = new CClientSock(this);

	if(m_pListenSock->Accept(*pClientSock)){
		//Receive data from here and close connection
		if(hbmScreen != NULL){
				DeleteObject(hbmScreen);
				hbmScreen = NULL;
		}

		hbmScreen = pClientSock->ReceiveImage();
		if(hbmScreen != NULL){
			OutputDebugString("Image returned sucessfully!\n");
			Invalidate(); //Update windows
		}

		pClientSock->Close();
	}
	
	delete pClientSock;
}