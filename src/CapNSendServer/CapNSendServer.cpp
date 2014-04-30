
// CapNSendServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CapNSendServer.h"
#include "ximage.h"
#include "CNSSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCapNSendServerApp

BEGIN_MESSAGE_MAP(CCapNSendServerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCapNSendServerApp construction

CCapNSendServerApp::CCapNSendServerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCapNSendServerApp object

CCapNSendServerApp theApp;


// CCapNSendServerApp initialization

BOOL CCapNSendServerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are storedmac 
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	if(__argc > 1)
		CaptureAndSend();
	else{
		CString msgtext;
		msgtext.Format("Usage: %s [IP address]\n", __argv[0]);
		MessageBox(NULL, msgtext, "Usage Error", 0);
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

// CapNSendServer [ip address] [port] [x] [y] [width] [height]
void CCapNSendServerApp::CaptureAndSend(){
	HDC hdcDesktop;
	HDC hdcMem;
	HBITMAP hbmScreen = NULL;
	BITMAP bmpScreen;
	CxImage* newImage = new CxImage();
	int screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	hdcDesktop = ::GetDC(NULL);
	
	hdcMem = CreateCompatibleDC(hdcDesktop);
	if(!hdcMem){
		MessageBox(NULL, "Cannot create compatible DC", "Failed", MB_OK);
		return;
	}
	hbmScreen = CreateCompatibleBitmap(hdcDesktop, screenWidth, screenHeight);
	if(!hbmScreen){
		MessageBox(NULL, "Cannot create compatible bitmap", "Failed", MB_OK);
		return;
	}

	SelectObject(hdcMem,hbmScreen);

	//unless bitblt need bitmapdc
	if(!BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcDesktop, 0, 0, SRCCOPY)){
		MessageBox(NULL, "Cannot do BitBlt", "Failed", MB_OK);
		return;
	}

	::GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

	//Convert BMP to PNG
	newImage->CreateFromHBITMAP(hbmScreen);
	//newImage->Save("image.png", CXIMAGE_FORMAT_PNG);

	long size = 0;
	BYTE* buffer = 0;
	newImage->Encode(buffer, size, CXIMAGE_FORMAT_PNG);
	
	//send buffer to server...
    //socket part
	//connect to server, send metadata(buffer size), followed by buffer, port 60601
	CCNSSocket m_cnssocket;
	m_cnssocket.Connect(__argv[1], 60601);
	m_cnssocket.SendImage(buffer, size);
	m_cnssocket.Close();

	// release all DC
	newImage->FreeMemory(buffer);
	newImage->Destroy();
	DeleteDC(hdcMem);
	if(hbmScreen != NULL)
		DeleteObject(hbmScreen);
	ReleaseDC(GetDesktopWindow(), hdcDesktop);
}