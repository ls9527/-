
// AyaTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAyaToolsApp:
// �йش����ʵ�֣������ AyaTools.cpp
//

class CAyaToolsApp : public CWinApp
{
public:
	CAyaToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAyaToolsApp theApp;