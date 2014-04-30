#pragma once
#include "ximage.h"

// CClientSock command target
typedef struct tagCNSMetaSock{
	long cmdcode;
    long buffsize;
} CNSMetaSock;

class CChildView;

class CClientSock : public CSocket
{
public:
	enum{SEND_IMAGE};
	CChildView* m_pChildView;

public:
	CClientSock(CChildView*);
	virtual ~CClientSock();

public:
	int SendImage(BYTE* buff, long buffsize);
	HBITMAP ReceiveImage();

};
