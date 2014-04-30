#pragma once
#include <afxsock.h>

typedef struct tagCNSMetaSock{
	long cmdcode;
    long buffsize;
} CNSMetaSock;

class CCNSSocket : public CSocket
{
public:
	enum{SEND_IMAGE};
	int SendImage(BYTE* buff, long buffsize);
};
