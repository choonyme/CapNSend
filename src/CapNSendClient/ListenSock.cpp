#include "stdafx.h"
//#include "CapNSendClient.h"
#include "ListenSock.h"
#include "ChildView.h"

CListenSock::CListenSock(CChildView *pChildView)
{
	m_pChildView = pChildView;
}

CListenSock::~CListenSock(void)
{
}

void CListenSock::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	m_pChildView->ProcessPendingAccept();
}
