
// Player UIDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Player UI.h"
#include "Player UIDlg.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "atlconv.h"
#include <string>
#include <sstream>
#include <time.h>
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PLAYERUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	//CListCtrl m_list;
	afx_msg void OnBnClickedButton6();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON6, &CAboutDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CPlayerUIDlg 대화 상자


CPlayerUIDlg::CPlayerUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlayerUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImage = NULL;
	m_pThread = NULL;
	m_bDo = TRUE;
	RECT rect = {0,0,200,300};
	playFlag = FALSE;
	capture = NULL;
	m_bPlayState = FALSE;
	height = 0;
	width = 0;
	fullFlag = FALSE;
	m_pKinect_Socket   = NULL;
	ip = "192.168.111.135";
	port = "9898";
	pathName = _T("");
}

void CPlayerUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//DDX_
}

BEGIN_MESSAGE_MAP(CPlayerUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPlayerUIDlg::OnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CPlayerUIDlg::OnClickedStop)
	ON_BN_CLICKED(IDC_BUTTON4, &CPlayerUIDlg::OnClickedPause)
	ON_STN_CLICKED(IDC_STATIC5, &CPlayerUIDlg::OnClickedFullScreen)
	ON_BN_CLICKED(IDC_BUTTON6, &CPlayerUIDlg::OnClickedConnect)
	ON_BN_CLICKED(IDC_BUTTON7, &CPlayerUIDlg::OnClickedDisconnect)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlayerUIDlg 메시지 처리기

BOOL CPlayerUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Init();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


void CPlayerUIDlg::Init()
{

	m_list.SetExtendedStyle(LVS_EX_GRIDLINES);

	m_list.InsertColumn(0, _T("Data"), LVCFMT_LEFT, 140, -1);
	m_list.InsertColumn(1, _T("X"), LVCFMT_CENTER, 109, -1);
	m_list.InsertColumn(2, _T("Y"), LVCFMT_CENTER, 109, -1);
	m_list.InsertColumn(3, _T("Z"), LVCFMT_CENTER, 109, -1);

	m_list.InsertItem(0, _T("Center_Pos"));
	m_list.InsertItem(1, _T("Rotation_Pos"));
	m_list.InsertItem(2, _T("Left_Pos"));
	m_list.InsertItem(3, _T("Right_Pos"));
	m_list.InsertItem(4, _T("Img_Left_Pos"));
	m_list.InsertItem(5, _T("Img_Right_Pos"));
	m_list.InsertItem(6, _T("Distance_Pos"));
}

void CPlayerUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPlayerUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(m_pImage)
		{
			CRect rt;
			CDC* pStaticDC = GetDlgItem(IDC_STATIC1)->GetDC();
			GetDlgItem(IDC_STATIC1)->GetClientRect(&rt);

			m_cImage.CopyOf(m_pImage);
			m_cImage.DrawToHDC(pStaticDC->m_hDC, &rt);
			ReleaseDC(pStaticDC);

		}
		CDialogEx::OnPaint(); //그려주기
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPlayerUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlayerUIDlg::OnClickedPlay()
{	

	TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.AVI, *.MP4, *.WMV, *.MKV) | *.AVI; *.MP4; *.WMV; *.MKV | 모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("*.AVI"), _T("video"), OFN_HIDEREADONLY, szFilter);

	if(dlg.DoModal() == IDOK)
	{
		pathName = dlg.GetPathName();

		if(playFlag == TRUE){
			memset(szFile, 0, sizeof(pathName.GetLength()));
		}
		else if(playFlag == FALSE){
			szFile = new char[pathName.GetLength()];
		}
		strcpy(szFile, CT2A(pathName));

	
		if(playFlag == FALSE){
			m_bPlayState = TRUE;
			capture = cvCreateFileCapture(szFile);
			getResolution(capture);
			CPlayerUIDlg::StartThread(); // 스레드 함수 호출
			playFlag = TRUE;
		}
		else if(playFlag == TRUE){
			m_bPlayState = FALSE;
			cvReleaseCapture(&capture);
			capture = cvCreateFileCapture(szFile);
			getResolution(capture);
			m_bPlayState = TRUE;
		}
		SetDlgItemText(IDC_STATIC13, pathName);
	}
}

void CPlayerUIDlg::getResolution(CvCapture* capture)
{
	CString strHeight;
	CString strWidth;

	height = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	width = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	strHeight.Format(_T("%d"), height);
	strWidth.Format(_T("%d"), width);

	SetDlgItemText(IDC_STATIC4, strWidth);
	SetDlgItemText(IDC_STATIC5, strHeight);
}

BOOL CPlayerUIDlg::DestroyWindow()
{
	if(m_pImage != NULL){
		m_pImage = NULL; // null 없이 release시 runtime 오류 발생!
		cvReleaseImage(&m_pImage);
	}

	m_cImage.Destroy();

	if(capture){
		capture = NULL;
		cvReleaseCapture(&capture);
	}
	CPlayerUIDlg::StopThread();
	Sleep(100);
	return CDialog::DestroyWindow();
}

void CPlayerUIDlg::OnClickedStop()
{
	if(playFlag == FALSE){
		GetDlgItem(IDC_BUTTON4)->SetWindowText(_T("일시정지"));
	}
	m_pImage = NULL;
	cvReleaseImage(&m_pImage);
	m_pThread->SuspendThread();
	capture = cvCreateFileCapture(szFile); 
	playFlag = FALSE;
	InvalidateRect(&rect, FALSE);
}


void CPlayerUIDlg::OnClickedPause()
{
	if(playFlag == TRUE){
		m_pImage = NULL;
		cvReleaseImage(&m_pImage);
		m_pThread->SuspendThread(); 
		playFlag = FALSE;
		GetDlgItem(IDC_BUTTON4)->SetWindowText(_T("재생")); 
		InvalidateRect(&rect, FALSE);
	}
	else if(playFlag == FALSE){
		m_pThread->ResumeThread();
		playFlag = TRUE;
		GetDlgItem(IDC_BUTTON4)->SetWindowText(_T("일시정지")); 
	}
}

// 반복함수
UINT CPlayerUIDlg::MyThreadFunc(LPVOID pThreadParam)
{
	THREADPARAM *pParam = (THREADPARAM*)pThreadParam;
	CPlayerUIDlg *pMyDlg = (CPlayerUIDlg*)pParam->pWnd;
	BOOL *pDo = pParam->pDo;
	delete pParam;

	while(*pDo)
	{
		pMyDlg->MyWorkFunc();
		Sleep(35);
	}
	return 0;
}

void CPlayerUIDlg::StartThread()
{
	THREADPARAM *pThreadParam = new THREADPARAM;
	pThreadParam->pWnd = this;
	pThreadParam->pDo = &m_bDo;

	m_pThread = AfxBeginThread(MyThreadFunc, pThreadParam); // 스레드 실행
}

void CPlayerUIDlg::StopThread()
{
	m_bDo = FALSE;
	Sleep(100);

	if(::WaitForSingleObject(m_pThread, INFINITE))
	{
		m_pThread = NULL;
	}
}

//반복 호출 함수
void CPlayerUIDlg::MyWorkFunc()
{	
	m_pImage = NULL;	
	if(m_bPlayState == TRUE)
	{
		m_pImage = cvQueryFrame(capture);
		fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS); 
		strFps.Format(_T("%.2lf"), fps);
		SetDlgItemText(IDC_STATIC3, strFps);
		InvalidateRect(&rect, FALSE);
	}
}

void CPlayerUIDlg::UpData()
{
	CString cen_pos_x;
	CString cen_pos_y;
	CString cen_pos_z;

	CString rotation_pos_x;
	CString rotation_pos_y;
	CString rotation_pos_z;

	CString left_pos_x;
	CString left_pos_y;
	CString left_pos_z;

	CString right_pos_x;
	CString right_pos_y;
	CString right_pos_z;

	CString img_left_pos_x;
	CString img_left_pos_y;
	CString img_left_pos_z;

	CString img_right_pos_x;
	CString img_right_pos_y;
	CString img_right_pos_z;

	CString distance_pos_x;
	CString distance_pos_y;
	CString distance_pos_z;

	cen_pos_x.Format(_T("%f"), m_pKinect_Socket->center_pos.x);
	cen_pos_y.Format(_T("%f"), m_pKinect_Socket->center_pos.y);
	cen_pos_z.Format(_T("%f"), m_pKinect_Socket->center_pos.z);

	rotation_pos_x.Format(_T("%f"), m_pKinect_Socket->rotation_pos.x);
	rotation_pos_y.Format(_T("%f"), m_pKinect_Socket->rotation_pos.y);
	rotation_pos_z.Format(_T("%f"), m_pKinect_Socket->rotation_pos.z);

	left_pos_x.Format(_T("%f"), m_pKinect_Socket->left_pos.x);
	left_pos_y.Format(_T("%f"), m_pKinect_Socket->left_pos.y);
	left_pos_z.Format(_T("%f"), m_pKinect_Socket->left_pos.z);

	right_pos_x.Format(_T("%f"), m_pKinect_Socket->right_pos.x);
	right_pos_y.Format(_T("%f"), m_pKinect_Socket->right_pos.y);
	right_pos_z.Format(_T("%f"), m_pKinect_Socket->right_pos.z);

	img_left_pos_x.Format(_T("%f"), m_pKinect_Socket->img_left_pos.x);
	img_left_pos_y.Format(_T("%f"), m_pKinect_Socket->img_left_pos.y);
	img_left_pos_z.Format(_T("%f"), m_pKinect_Socket->img_left_pos.z);

	img_right_pos_x.Format(_T("%f"), m_pKinect_Socket->img_right_pos.x);
	img_right_pos_y.Format(_T("%f"), m_pKinect_Socket->img_right_pos.y);
	img_right_pos_z.Format(_T("%f"), m_pKinect_Socket->img_right_pos.z);

	distance_pos_x.Format(_T("%f"), m_pKinect_Socket->distance_pos.x);
	distance_pos_y.Format(_T("%f"), m_pKinect_Socket->distance_pos.y);
	distance_pos_z.Format(_T("%f"), m_pKinect_Socket->distance_pos.z);
	
	m_list.SetItem(0, 1, LVIF_TEXT, cen_pos_x, 0,0,0,NULL);
	m_list.SetItem(0, 2, LVIF_TEXT, cen_pos_y, 0,0,0,NULL);
	m_list.SetItem(0, 3, LVIF_TEXT, cen_pos_z, 0,0,0,NULL);

	m_list.SetItem(1, 1, LVIF_TEXT, rotation_pos_x, 0,0,0,NULL);
	m_list.SetItem(1, 2, LVIF_TEXT, rotation_pos_y, 0,0,0,NULL);
	m_list.SetItem(1, 3, LVIF_TEXT, rotation_pos_z, 0,0,0,NULL);

	m_list.SetItem(2, 1, LVIF_TEXT, left_pos_x, 0,0,0,NULL);
	m_list.SetItem(2, 2, LVIF_TEXT, left_pos_y, 0,0,0,NULL);
	m_list.SetItem(2, 3, LVIF_TEXT, left_pos_z, 0,0,0,NULL);

	m_list.SetItem(3, 1, LVIF_TEXT, right_pos_x, 0,0,0,NULL);
	m_list.SetItem(3, 2, LVIF_TEXT, right_pos_y, 0,0,0,NULL);
	m_list.SetItem(3, 3, LVIF_TEXT, right_pos_z, 0,0,0,NULL);

	m_list.SetItem(4, 1, LVIF_TEXT, img_left_pos_x, 0,0,0,NULL);
	m_list.SetItem(4, 2, LVIF_TEXT, img_left_pos_y, 0,0,0,NULL);
	m_list.SetItem(4, 3, LVIF_TEXT, img_left_pos_z, 0,0,0,NULL);

	m_list.SetItem(5, 1, LVIF_TEXT, img_right_pos_x, 0,0,0,NULL);
	m_list.SetItem(5, 2, LVIF_TEXT, img_right_pos_y, 0,0,0,NULL);
	m_list.SetItem(5, 3, LVIF_TEXT, img_right_pos_z, 0,0,0,NULL);

	m_list.SetItem(6, 1, LVIF_TEXT, distance_pos_x, 0,0,0,NULL);
	m_list.SetItem(6, 2, LVIF_TEXT, distance_pos_y, 0,0,0,NULL);
	m_list.SetItem(6, 3, LVIF_TEXT, distance_pos_z, 0,0,0,NULL);

	//UpdateData(TRUE);
}

void CPlayerUIDlg::OnClickedFullScreen()
{
	fullFlag = TRUE;
	cvNamedWindow("TestMedia", CV_WINDOW_NORMAL);
	cvSetWindowProperty("TestMedia", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
}


void CPlayerUIDlg::OnClickedConnect()
{
	m_pKinect_Socket = new CClientSocket();
	if(!m_pKinect_Socket->Connect(ip, port)){
		AfxMessageBox(L"키넥트 서버가 활성화 되지 않았습니다.");
		m_pKinect_Socket->close();
		delete m_pKinect_Socket;
	}
	else{
		AfxMessageBox(L"키넥트 서버와 연결이 되었습니다.");
		SetTimer(123,100, NULL);
		//m_pKinect_Socket->SetControl(this);
		m_pKinect_Socket->m_pFlg = TRUE;
	}
}


void CPlayerUIDlg::OnClickedDisconnect()
{
	KillTimer(123);
	m_pKinect_Socket->close();
	//delete m_pKinect_Socket;
}


void CAboutDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
}


void CPlayerUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 123:
		UpData();
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
