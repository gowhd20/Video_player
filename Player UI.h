
// Player UI.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPlayerUIApp:
// �� Ŭ������ ������ ���ؼ��� Player UI.cpp�� �����Ͻʽÿ�.
//

class CPlayerUIApp : public CWinApp
{
public:
	CPlayerUIApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPlayerUIApp theApp;