#pragma once

class CChildView;

class CListenSock : public CSocket
{
//Operations
public:
	CListenSock(CChildView *pChildView); //as server
	~CListenSock(void);
	virtual void OnAccept(int nErrorCode);


//Attributes
public:
	CChildView* m_pChildView;

};