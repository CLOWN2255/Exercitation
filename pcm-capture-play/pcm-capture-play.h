
// pcm-capture-play.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CpcmcaptureplayApp:
// �йش����ʵ�֣������ pcm-capture-play.cpp
//

class CpcmcaptureplayApp : public CWinAppEx
{
public:
	CpcmcaptureplayApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CpcmcaptureplayApp theApp;