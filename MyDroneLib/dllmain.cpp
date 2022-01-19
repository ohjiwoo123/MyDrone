// dllmain.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include <afxwin.h>
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE MyDroneLibDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// lpReserved를 사용하는 경우 다음을 제거하십시오.
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MyDroneLib.DLL을 초기화하고 있습니다.\n");

		// 확장 DLL을 한 번만 초기화합니다.
		if (!AfxInitExtensionModule(MyDroneLibDLL, hInstance))
			return 0;

		// 이 DLL을 리소스 체인에 삽입합니다.
		// 참고: 이 확장 DLL이 MFC 응용 프로그램이
		//  아닌 ActiveX 컨트롤과 같은 MFC 기본 DLL에
		//  의해 명시적으로 링크되어 있는 경우에는
		//  DllMain에서 이 줄을 제거하고, 제거한 줄은 이 확장 DLL에서
		//  내보낸 별도의 함수에 추가합니다.
		//  그런 다음 이 확장 DLL을 사용하는 기본 DLL은
		//  해당 함수를 명시적으로 호출하여 이 확장 DLL을 추가해야 합니다.
		//  그렇지 않으면 CDynLinkLibrary 개체가
		//  기본 DLL의 리소스 체인에 추가되지 않으므로
		//  심각한 문제가 발생합니다.

		new CDynLinkLibrary(MyDroneLibDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MyDroneLib.DLL을 종료하고 있습니다.\n");

		// 소멸자가 호출되기 전에 라이브러리를 종료합니다.
		AfxTermExtensionModule(MyDroneLibDLL);
	}
	return 1;   // 확인
}

/////////////////////////////////////////////////////////////////////////
//0: 초기화 전(닫힌 상태)
//1: 초기화 완료
//2: 정상적 운영 가능 상태
//3: 설정 변경 중
int g_nDevState = 0;




/////////////////////////////////////////////////////////////////////////
//에러코드
//0: 정상
//10: 장치가 초기화 되지 않았음.
//20: 센서 오류
//30: 설정 실패
int g_nErrorCode = 10;

/////////////////////////////////////////////////////////////////////////
//1. 장치 초기화
extern "C" __declspec(dllexport)
BOOL InitMyDev(int nLevel)
{
	::Sleep(10);
	if (rand() % 10 == 5)
	{
		g_nErrorCode = 10;		//초기화 오류
		return FALSE;
	}
	g_nErrorCode = 0;			//정상상태
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//2. 장치 에러 값 구하기
extern "C" __declspec(dllexport)
int GetMyDevError(void)
{
	return g_nErrorCode;
}

/////////////////////////////////////////////////////////////////////////
//3. 장치의 현재 좌표 구하기
extern "C" __declspec(dllexport)
BOOL GetMyDevPosition(int* x, int* y)
{
	if (rand() % 50 == 5)		//2% 확률로 실패
	{
		::Sleep(100);
		g_nErrorCode = 20;

		return FALSE;
	}

	*x = rand() % 101;
	*y = rand() % 101;
	g_nErrorCode = 0;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//4. 장치 해지
extern "C" __declspec(dllexport)
void ReleaseMyDev(void)
{
	g_nErrorCode = 10;
}
