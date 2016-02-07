
// Player UIDlg.h : 헤더 파일
//

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "CvvImage.h"
#include "ClientSocket.h"

#pragma once


// CPlayerUIDlg 대화 상자
class CPlayerUIDlg : public CDialogEx
{
// 생성입니다.
public:
	CPlayerUIDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PLAYERUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;

public:
	CvCapture* capture;
	IplImage* m_pImage;
	CvvImage m_cImage;
	double fps;
	CWinThread *m_pThread;
	BOOL m_bDo;
	RECT rect;
	BOOL playFlag;
	CString pathName;
	char *szFile;
	BOOL m_bPlayState;
	CString strFps;
	double property;
	int height;
	int width;
	BOOL fullFlag;
	CClientSocket *m_pKinect_Socket;

	char* ip;
	char* port;

	static UINT MyThreadFunc(LPVOID pThreadParam);
	void StartThread();
	void StopThread();
	void MyWorkFunc();
	BOOL DestroyWindow();
	void getResolution(CvCapture* capture);

	afx_msg void OnClickedPlay();
	afx_msg void OnClickedStop();
	afx_msg void OnClickedPause();
	afx_msg void OnClickedFullScreen();
	afx_msg void OnClickedConnect();
	afx_msg void OnClickedDisconnect();

public:
	void Init();
	void UpData();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


typedef struct tagThreadParam
{
	CWnd *pWnd;
	BOOL *pDo;
}THREADPARAM;
