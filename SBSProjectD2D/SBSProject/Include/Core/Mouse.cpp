#include "Mouse.h"

DEF_SINGLE(CMouse);

CMouse::CMouse():
	m_bIsClicked(false),
	m_bMouseUp(false)/*,
	m_hEditBox(NULL)*/
{
}

CMouse::~CMouse()
{
}

bool CMouse::Init()
{
	return true;
}

void CMouse::Input(float fDeltaTime)
{
}

int CMouse::Update(HWND hWnd, float fDeltaTime)
{
	POSITION tPos = m_tPos;
	GetCursorPos(&m_tPos);
	ScreenToClient(hWnd, &m_tPos);
	POSITION tPos2 = m_tPos;
	m_tMoveDist = tPos2 - tPos;

	return 0;
}

int CMouse::LateUpdate(float fDeltaTime)
{
	m_bMouseUp = false;

	return 0;
}

void CMouse::Render(HDC hDC, float fDeltaTime)
{
	wchar_t str[32] = {};
	wsprintf(str, L"x: %d, y: %d", m_tPos.x, m_tPos.y);
	TextOut(hDC, m_tPos.x, m_tPos.y, str, lstrlen(str));
//#ifdef _DEBUG
//	gotoxy(10, 0);
//	if (m_bIsClicked)
//		printf("Mouse : Clicked");
//	else if (m_bIsClicked)
//		printf("Mouse : MouseUP");
//	else
//		printf("Mouse :        ");
//#endif
}
