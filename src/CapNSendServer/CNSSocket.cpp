#include "StdAfx.h"
#include "CNSSocket.h"

int CCNSSocket::SendImage(BYTE* buff, long buffsize)
{
	CNSMetaSock mCNSMetaSock;
	int cbLeftToSend, cbByteRet;

	mCNSMetaSock.buffsize = buffsize;
	mCNSMetaSock.cmdcode = SEND_IMAGE;

	cbLeftToSend = sizeof(CNSMetaSock);
	do
	{
		BYTE* bp = (BYTE*)(&mCNSMetaSock) + sizeof(CNSMetaSock) - cbLeftToSend;
		cbByteRet = Send(bp, cbLeftToSend);

		if(cbByteRet == SOCKET_ERROR || cbByteRet == 0){
			OutputDebugString("Error! Cannot send image meta data!\n");
			return -1;
		}

		cbLeftToSend -= cbByteRet;
	}while(cbLeftToSend > 0);

	cbLeftToSend = buffsize;
	do{
		BYTE* bp = buff + buffsize - cbLeftToSend;
		cbByteRet = Send(bp, cbLeftToSend);

		if(cbByteRet == SOCKET_ERROR || cbByteRet == 0){
			OutputDebugString("Error! Cannot send image!\n");
			return -1;
		}

		cbLeftToSend -= cbByteRet;
	}while(cbLeftToSend > 0);
	return 0;
}
