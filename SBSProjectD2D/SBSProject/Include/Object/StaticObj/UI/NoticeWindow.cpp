#include "NoticeWindow.h"
#include "../../../Resources/ResourcesManager.h"
#include "LoadingObj.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"

CNoticeWindow::CNoticeWindow():
	m_pButton(NULL),
	m_eWindowState(WS_WRONGID),
	m_pConnecting(NULL)
{
}

CNoticeWindow::CNoticeWindow(const CNoticeWindow & obj):
	CStaticObj(obj)
{
}

CNoticeWindow::~CNoticeWindow()
{
	SAFE_RELEASE(m_pButton);
	for (int i = 0; i < WS_END; i++)
		SAFE_RELEASE(m_pTextures[i]);
	SAFE_RELEASE(m_pConnecting);
}

bool CNoticeWindow::Init()
{
	m_pButton = new CButton;

	if (!m_pButton->Init())
		return NULL;

	//SetPos(387.f, 313.f);
	SetPos(387.f, 163.f);
	SetSize(249.f, 142.f);
	SetEnable(false);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	// 배경
	SetTexture("NoticeWindow", L"UI/NoticeWindow/Notice_Background.png", tOrigin, tLT, POSITION(249.f, 142.f));
	
	// 취소버튼
	m_tCancelButtonPos = POSITION(101.f, 106.f);// 위치
	m_pButton->LoadButton("CancelButton", L"UI/NoticeWindow/CancelButton/", tOrigin, tLT, POSITION(50.f, 23.f));
	m_pButton->SetIconFunction(this, &CNoticeWindow::DisableWindow);

	// 문구
	m_tNoticePos = POSITION(17.f, 13.f);// 위치
	m_pTextures[WS_WRONGID] = GET_SINGLE(CResourcesManager)->LoadTexture("WrongID", L"UI/NoticeWindow/WrongID.png", tOrigin, _SIZE(215.f, 86.f), tLT, POSITION(215.f, 86.f));
	if (!m_pTextures[WS_WRONGID])
		return false;
	m_pTextures[WS_EXISTID] = GET_SINGLE(CResourcesManager)->LoadTexture("ExistID", L"UI/NoticeWindow/ExistID.png", tOrigin, _SIZE(215.f, 86.f), tLT, POSITION(215.f, 86.f));
	if (!m_pTextures[WS_EXISTID])
		return false;
	m_pTextures[WS_WRONGPW] = GET_SINGLE(CResourcesManager)->LoadTexture("WrongPW", L"UI/NoticeWindow/WrongPW.png", tOrigin, _SIZE(215.f, 86.f), tLT, POSITION(215.f, 86.f));
	if (!m_pTextures[WS_WRONGPW])
		return false;

	m_pTextures[WS_CONNECTING] = GET_SINGLE(CResourcesManager)->LoadTexture("Loading", L"UI/NoticeWindow/Loading.png", tOrigin, _SIZE(249.f, 142.f), tLT, POSITION(249.f, 142.f));
	if (!m_pTextures[WS_CONNECTING])
		return false;

	// 로딩
	m_tConnectingPos = POSITION(87.f, 34.f); // 위치
	m_pConnecting = new CLoadingObj;
	if (!m_pConnecting->Init())
		return false;

	return true;
}

void CNoticeWindow::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);

	if (KEYPRESS("Enter"))
	{
		DisableWindow();
	}
}

int CNoticeWindow::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	// 창 움직이기 가능
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();
	RECTANGLE tRect = RECTANGLE(m_tPos.x, m_tPos.y,
		m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);

	// 마우스 변화 거리 반영
	if (bIsClicked && PointInRect(tRect, tMousePos))
	{
		POSITION tMoveDist = GET_SINGLE(CMouse)->GetMoveDist();
		m_tPos += tMoveDist;
	}

	if (m_pButton)
	{
		m_pButton->SetIconPos(m_tPos + m_tCancelButtonPos);
		if (m_pButton->IconUpdate(fDeltaTime) == 1)
			m_pButton->UseIcon();
	}

	if (m_pConnecting)
	{
		m_pConnecting->SetPos(m_tPos + m_tConnectingPos);
		if (m_pConnecting->GetEnable())
			m_pConnecting->Update(fDeltaTime);
	}

	return 0;
}

int CNoticeWindow::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CNoticeWindow::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CNoticeWindow::Render(ID2D1GdiInteropRenderTarget * pGDIRenderTarget, ID2D1HwndRenderTarget * pRenderTarget, float fDeltaTime)
{
	//CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	if (m_pTexture)
	{
		ID2D1Bitmap* pD2DBitmap = m_pTexture->GetBitmap();
		POSITION tLT = m_pTexture->GetLT();
		POSITION tRB = m_pTexture->GetRB();

		// tPos 기준 texture 사각형 범위
		D2D1_RECT_F tRect;
		tRect.left = m_tPos.x + tLT.x;
		tRect.top = m_tPos.y + tLT.y;
		tRect.right = m_tPos.x + tRB.x;
		tRect.bottom = m_tPos.y + tRB.y;

		pRenderTarget->DrawBitmap(pD2DBitmap, &tRect);
	}

	POSITION tPos = m_tPos;
	if (m_eWindowState != WS_CONNECTING)
		tPos += m_tNoticePos;
	m_pTextures[m_eWindowState]->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);

	if (m_pConnecting)
	{
		if (m_pConnecting->GetEnable())
			m_pConnecting->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	if (m_pButton)
		m_pButton->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CNoticeWindow * CNoticeWindow::Clone()
{
	return new CNoticeWindow(*this);
}

void CNoticeWindow::SetWindowState(WINDOW_STATE eWindowState)
{
	m_eWindowState = eWindowState;
	SetEnable(true);

	// connecting이라면 connecting을 활성화
	if (m_eWindowState == WS_CONNECTING)
		m_pConnecting->SetEnable(true);
	else
		m_pConnecting->SetEnable(false);
}

void CNoticeWindow::DisableWindow()
{
	SetEnable(false);
	// 위치 제자리로
	//SetPos(387.f, 313.f);
	SetPos(387.f, 163.f);
}