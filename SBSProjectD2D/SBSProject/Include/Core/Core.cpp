#include "Core.h"
#include "Timer.h"
#include "Input.h"
#include "../Scene/SceneManager.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"
#include "../Collider/CollisionManager.h"
#include "Camera.h"
#include "BackCam.h"
#include "Mouse.h"
#include "../ItemManager/ItemManager.h"
#include "../DamageManager/DamageManager.h"
#include "ServerManager.h"
#include "../Object/StaticObj/UI/UIMain.h"
#include "../Object/StaticObj/EditBox/EditBox.h"

DEF_SINGLE(CCore);
bool CCore::m_bLoop = true;
bool CCore::m_bIsActivated = true;

CCore::CCore()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(13013);
}

CCore::~CCore()
{
	DESTROY_SINGLE(CServerManager);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CDamageManager);
	DESTROY_SINGLE(CCamera);
	DESTROY_SINGLE(CBackCam);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CResourcesManager);
	DESTROY_SINGLE(CInput);	
	DESTROY_SINGLE(CMouse);
	DESTROY_SINGLE(CItemManager);

//#ifdef _DEBUG
//	FreeConsole();
//#endif

	ReleaseDC(m_hWnd, m_hDC);
}

void CCore::ChangeWindowRes(int iWidth, int iHeight)
{
	m_tRS.iW = iWidth;
	m_tRS.iH = iHeight;

	RECT rect = { 0, 0, m_tRS.iW, m_tRS.iH };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);
	GET_SINGLE(CCamera)->SetClientResolution(m_tRS);
	GET_SINGLE(CBackCam)->SetClientResolution(m_tRS);

	D2D1_SIZE_U tRect;
	tRect.width = 1366;
	tRect.height = 768;
	GET_SINGLE(CResourcesManager)->GetRenderTarget()->Resize(tRect);
}

bool CCore::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	MyRegisterClass();

	//m_tRS.iW = 1366;
	m_tRS.iW = 1024;
	m_tRS.iH = 768;

	if (!Create())
		return false;

	m_hDC = GetDC(m_hWnd);
	
	if (!GET_SINGLE(CServerManager)->Init())
		return false;

	if (!GET_SINGLE(CTimer)->Init(m_hWnd))
		return false;

	if (!GET_SINGLE(CResourcesManager)->Init(hInst, m_hDC, m_hWnd))
		return false;

	if (!GET_SINGLE(CInput)->Init(m_hWnd))
		return false;

	if (!GET_SINGLE(CCamera)->Init(POSITION(0.f, 0.f), m_tRS, RESOLUTION(0, 0)))
		return false;

	if (!GET_SINGLE(CBackCam)->Init(POSITION(0.f, 0.f), m_tRS, RESOLUTION(0, 0)))
		return false;

	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	if (!GET_SINGLE(CMouse)->Init())
		return false;

	if (!GET_SINGLE(CItemManager)->Init())
		return false;

	if (!GET_SINGLE(CDamageManager)->Init())
		return false;

	m_fFPS = 0.f;

//#ifdef _DEBUG
//	if (AllocConsole())
//	{
//		freopen("CONIN$", "rb", stdin);
//		freopen("CONOUT$", "wb", stdout);
//		freopen("CONOUT$", "wb", stderr);
//
//		// cout / cin / cerr 사용시 주석 해제
//		//std::ios::wync_with_stdio();
//	}
//#endif
	// 사용시 printf 사용 가능

	return true;
}

int CCore::Run()
{
	MSG msg;

	while (m_bLoop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			Logic();
		}
	}

	return 0;
}

void CCore::Logic()
{
	GET_SINGLE(CTimer)->Update();

	float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();
	
	Input(fDeltaTime);
	Update(fDeltaTime);
	LateUpdate(fDeltaTime);
	Collision(fDeltaTime);
	Render(fDeltaTime);
}

void CCore::Input(float fDeltaTime)
{
	if (CCore::m_bIsActivated)
		GET_SINGLE(CInput)->Update(fDeltaTime);
	
	GET_SINGLE(CSceneManager)->Input(fDeltaTime);
	GET_SINGLE(CCamera)->Input(fDeltaTime);
	GET_SINGLE(CBackCam)->Input(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
	GET_SINGLE(CMouse)->Update(m_hWnd, fDeltaTime);
	GET_SINGLE(CSceneManager)->Update(fDeltaTime);
	GET_SINGLE(CDamageManager)->Update(fDeltaTime);
	GET_SINGLE(CCamera)->Update(fDeltaTime);
	GET_SINGLE(CBackCam)->Update(fDeltaTime);
	
	return 0;
}

int CCore::LateUpdate(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->LateUpdate(fDeltaTime);
	GET_SINGLE(CMouse)->LateUpdate(fDeltaTime);
	return 0;
}

void CCore::Collision(float fDeltaTime)
{
	GET_SINGLE(CSceneManager)->Collision(fDeltaTime);
	GET_SINGLE(CCollisionManager)->Collision(fDeltaTime);
}

void CCore::Render(float fDeltaTime)
{
	//ID2D1DCRenderTarget* pRenderTarget = GET_SINGLE(CResourcesManager)->GetRenderTarget();
		
	// d2d 렌더 시작
	ID2D1HwndRenderTarget* pRenderTarget = GET_SINGLE(CResourcesManager)->GetRenderTarget();
	pRenderTarget->BeginDraw();
	// 검은바탕으로 clear
	pRenderTarget->Clear(D2D1_COLOR_F(ColorF(ColorF::Black)));

	// GDIInteropRenderTarget의 GetDC는 BeginDraw ~ EndDraw 사이에서만 사용 가능
	// HDC 호출은 사용하는 곳에서 따로해주기
	ID2D1GdiInteropRenderTarget* pGDIRenderTarget = GET_SINGLE(CResourcesManager)->GetGDIRenderTarget();	

	GET_SINGLE(CSceneManager)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	GET_SINGLE(CDamageManager)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	pRenderTarget->EndDraw();

	//GET_SINGLE(CMouse)->Render(m_hDC, fDeltaTime);
}

ATOM CCore::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL; // MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = L"SBSProject";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CCore::Create()
{
	m_hWnd = CreateWindowEx(0, L"SBSProject", L"SBSProject",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);
	
	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT rect = { 0, 0, m_tRS.iW, m_tRS.iH };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_MOUSEMOVE:
	{

	}
	break;

	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE || // 창 비활성화
			HIWORD(wParam) == 32) // 또는 창 최소화
		{
			GET_SINGLE(CCore)->SetActivate(false);
			GET_SINGLE(CSceneManager)->SetActivateChanged(true);
		}
		else // 활성화
		{
			GET_SINGLE(CCore)->SetActivate(true);
			GET_SINGLE(CSceneManager)->SetActivateChanged(true);
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{
		GET_SINGLE(CMouse)->SetClicked(true);
	}
	break;

	case WM_LBUTTONUP:
	{
		GET_SINGLE(CMouse)->SetClicked(false);
		GET_SINGLE(CMouse)->SetMouseUp(true);
	}
	break;

	// alt키 정지 막기
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_MENU:
			break;
		}
		break;

	case WM_LBUTTONDBLCLK:
	{

	}
	break;

	case WM_DESTROY:
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void CCore::Exit()
{
	m_bLoop = false;
	GET_SINGLE(CSceneManager)->StopBGM();
}

HWND CCore::CreateEditBox(int id, float l, float t, float r, float b, bool bIsPW)
{
	if (bIsPW)
		return CreateWindowEx(0, L"edit", NULL,
			/*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD,
			l, t, r - l, b - t,
			m_hWnd, (HMENU)id, m_hInst, NULL);
	else
		return CreateWindowEx(0, L"edit", NULL,
			/*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, l, t, r - l, b - t,
			m_hWnd, (HMENU)id, m_hInst, NULL);
}

HWND CCore::CreateEditBox(int id, RECTANGLE tRect, bool bIsPW)
{
	if (bIsPW)
		return CreateWindowEx(0, L"edit", NULL,
			/*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_PASSWORD,
			tRect.l, tRect.t, tRect.r - tRect.l, tRect.b - tRect.t,
			m_hWnd, (HMENU)id, m_hInst, NULL);
	else
		return CreateWindowEx(0, L"edit", NULL,
			/*WS_BORDER |*/ WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
			tRect.l, tRect.t, tRect.r - tRect.l, tRect.b - tRect.t,
			m_hWnd, (HMENU)id, m_hInst, NULL);
}