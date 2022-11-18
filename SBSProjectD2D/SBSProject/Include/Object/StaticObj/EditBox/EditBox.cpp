#include "EditBox.h"
#include "../../../Core/Core.h"
#include "../../../Core/Input.h"
//#include "../../../Resources/ResourcesManager.h"

CEditBox::CEditBox():
	m_hEditBox(NULL),
	m_iId(0),
	m_strText(NULL),
	m_bEnter(false)
{

}

CEditBox::CEditBox(const CEditBox & obj) :
	CStaticObj(obj)
{
}

CEditBox::~CEditBox()
{
	if (m_hEditBox)
		DestroyWindow(m_hEditBox);

	m_strText = NULL;
}

HWND CEditBox::CreateEditBox(int id, RECTANGLE tRect, char* pBuffer, bool bIsPW)
{	
	m_iId = id;
	SetPos(tRect.l, tRect.t);
	SetSize(tRect.r- tRect.l, tRect.b - tRect.l);

	m_strText = pBuffer;

	if (m_hEditBox)
		CloseWindow(m_hEditBox);

	m_hEditBox = GET_SINGLE(CCore)->CreateEditBox(id, tRect, bIsPW);

	return m_hEditBox;
}

HWND CEditBox::CreateEditBox(int id, float l, float t, float r, float b, char* pBuffer, bool bIsPW)
{
	m_iId = id;
	SetPos(l, t);
	SetSize(r - l, b - l);

	m_strText = pBuffer;

	if (m_hEditBox)
		CloseWindow(m_hEditBox);

	m_hEditBox = GET_SINGLE(CCore)->CreateEditBox(id, l, t, r, b, bIsPW);

	return m_hEditBox;
}

void CEditBox::Reset()
{
	DestroyWindow(m_hEditBox);
}

bool CEditBox::Init()
{
	SetPivot(0.f, 0.f);

	return true;
}

void CEditBox::Input(float fDeltaTime)
{
	CObj::Input(fDeltaTime);

	if (KEYPRESS("Enter") && !m_bEnter)
	{
		m_bEnter = true;
	}
}

int CEditBox::Update(float fDeltaTime)
{
	CObj::Update(fDeltaTime);

	if (m_strText)
	{
		GetWindowTextA(m_hEditBox, m_strText, MAX_MESSAGE_LEN);
	}

	return 0;
}

int CEditBox::LateUpdate(float fDeltaTime)
{
	CObj::LateUpdate(fDeltaTime);
	return 0;
}

void CEditBox::Collision(float fDeltaTime)
{
	CObj::Collision(fDeltaTime);
}

void CEditBox::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CEditBox * CEditBox::Clone()
{
	return new CEditBox(*this);
}