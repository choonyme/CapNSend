// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "CapNSendClient.h"
#include "ClientSock.h"
#include "ChildView.h"

// CClientSock

CClientSock::CClientSock(CChildView* pChildView)
{
	m_pChildView = pChildView;
}

CClientSock::~CClientSock()
{
}


// CClientSock member functions
int CClientSock::SendImage(BYTE* buff, long buffsize)
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

HBITMAP CClientSock::ReceiveImage()
{
	CNSMetaSock pCNSMetaSock;
	BYTE* pImageBuff;
	int cbLeftToReceive, cbByteRet;
	HBITMAP hbmp = NULL;
	
	cbLeftToReceive = sizeof(CNSMetaSock);
	do
	{
		BYTE* bp = (BYTE*)(&pCNSMetaSock) + sizeof(CNSMetaSock) - cbLeftToReceive;
		cbByteRet = Receive(bp, cbLeftToReceive);

		if(cbByteRet == SOCKET_ERROR || cbByteRet == 0){
			OutputDebugString("Error! Cannot receive image meta data!\n");
			return NULL;
		}

		cbLeftToReceive -= cbByteRet;
	}while(cbLeftToReceive > 0);

	CString string;
	string.Format("Image size is %d\n", pCNSMetaSock.buffsize);
	OutputDebugString(string);
	pImageBuff = new BYTE[pCNSMetaSock.buffsize + 1];
	cbLeftToReceive = pCNSMetaSock.buffsize;
	do
	{
		BYTE* bp = pImageBuff + pCNSMetaSock.buffsize - cbLeftToReceive;
		cbByteRet = Receive(bp, cbLeftToReceive);

		if(cbByteRet == SOCKET_ERROR || cbByteRet == 0){
			OutputDebugString("Error! Cannot receive image!\n");
			return NULL;
		}

		cbLeftToReceive -= cbByteRet;
	}while(cbLeftToReceive > 0);

	//Can do proper conversion into PNG image and return as BITMAP
	CxImage* newImage = new CxImage();
	if(newImage->Decode(pImageBuff, pCNSMetaSock.buffsize, CXIMAGE_FORMAT_PNG)){
		//newImage->Save("received.png", CXIMAGE_FORMAT_PNG);
	}else{
		newImage->Destroy();
		return NULL;
	}

	hbmp = newImage->MakeBitmap();

	delete pImageBuff;
	newImage->Destroy();

	return hbmp;
}