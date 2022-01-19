
// MyDroneAppDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MyDroneApp.h"
#include "MyDroneAppDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyDroneAppDlg 대화 상자



CMyDroneAppDlg::CMyDroneAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDRONEAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	nLevel = 0;
}

void CMyDroneAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDroneAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDroneAppDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDroneAppDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyDroneAppDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMyDroneAppDlg 메시지 처리기

BOOL CMyDroneAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyDroneAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyDroneAppDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyDroneAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#include "MyDroneLib.h"
#pragma comment(lib,"MyDroneLib")

typedef struct MyData {
	int x;
	int y;
}MyData;

MyData g_array[100];

int nIndex;
int nCount;

HANDLE g_hCalc;
HANDLE g_hWrite;

DWORD WINAPI WriteThread(LPVOID p)
{

	HANDLE hPipe;

	for (;;)
	{
		if (WaitNamedPipe(_T("\\\\.\\pipe\\ValueofXY"), NMPWAIT_WAIT_FOREVER) == TRUE)
		{
			hPipe =
				CreateFile(_T("\\\\.\\pipe\\ValueofXY"),
					GENERIC_READ | GENERIC_WRITE,
					0, NULL, OPEN_EXISTING, 0, NULL);
			if (hPipe != INVALID_HANDLE_VALUE) break;
		}
	}

	// 서비스 요청 

	DWORD dwRead, dwWritten;
	BOOL bSuccess;
	CString strTemp2;
	MyData* arWork = (MyData*)p;

	for (int i = 0;i<100;i++)
	{
		strTemp2.Format(_T("x=%d, y=%d\n"), arWork[i].x, arWork[i].y);
		bSuccess = WriteFile(hPipe, &arWork[i],
			8, &dwWritten, NULL);

		if ((bSuccess == FALSE) || (dwWritten == 0))
		{
			break;
		}
	}
	return 0;
}



DWORD WINAPI CalcThread(LPVOID p)
{
	// 곱하기 2 수행하는 쓰레드 
	MyData* arWork = (MyData*)p;
	for (;;)
	{
		Sleep(500);
		arWork[nIndex].x = arWork[nIndex].x * 2;
		arWork[nIndex].y = arWork[nIndex].y * 2;

		g_hWrite = CreateThread(NULL, 0, WriteThread, arWork, 0, 0);

	}
	return 0;
}

DWORD WINAPI MonitorThread(LPVOID p)
{
	CString strTemp;
	int x = 0;
	int y = 0;
	nCount = 0;
	nIndex = 0;

	CWnd *pWnd = (CWnd*)p;

	for (;;)
	{
		Sleep(100);

		if (::GetMyDevPosition(&x, &y))
		{
			strTemp.Format(_T("x=%d, y=%d\n"), x, y);
		}			
		else
			strTemp.Format(_T("에러발생 : %d\n"), GetMyDevError());

		::OutputDebugString(strTemp);
		pWnd->SetWindowText(strTemp);
		// 1. 배열(100개)를 모은다
		g_array[nIndex].x = x;
		g_array[nIndex].y = y;
		
		nIndex = nIndex + 1;
		// 2. 100개가 모아졌는가?
		if (nIndex == 100)
		{
			// 작업 복사본을 생성함
			MyData* arCopy = (MyData*)malloc(sizeof(g_array));	// 동적메모리 실행중에 잡는다.
			memcpy(arCopy, g_array, sizeof(g_array));

			DWORD ID;
			// 제어할 의사가 없다.
			CloseHandle(CreateThread(NULL, 0, CalcThread, arCopy, 0, &ID));
			// nCount 초기화 
			nIndex = 0;
		}
	}
	return 0;
}

HANDLE g_hMonitoring;
void CMyDroneAppDlg::OnBnClickedButton1()	// start 
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	InitMyDev(10);
	g_hMonitoring = CreateThread(NULL, 0, MonitorThread, this, NULL, NULL);
	
}


void CMyDroneAppDlg::OnBnClickedButton2()	// suspend
{
	static BOOL bToggle = TRUE;	
	if (bToggle)
	{
		SuspendThread(g_hMonitoring);
		bToggle = FALSE;
		SetDlgItemText(IDC_BUTTON2, _T("Resume"));
	}
	else
	{
		ResumeThread(g_hMonitoring);
		bToggle = TRUE;
		SetDlgItemText(IDC_BUTTON2, _T("Suspend"));
	}
}


void CMyDroneAppDlg::OnBnClickedButton3()	// end
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	ReleaseMyDev();
	TerminateThread(g_hMonitoring, 0);
	CloseHandle(g_hMonitoring);
}
