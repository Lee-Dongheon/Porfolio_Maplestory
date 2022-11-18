#include "TitleUI.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Scene/SceneManager.h"
#include "../EditBox/EditBox.h"
#include "../../../Core/Input.h"
#include "../../../Core/ServerManager.h"
#include "../../../Resources/ResourcesManager.h"
#include "NoticeWindow.h"

bool CTitleUI::m_bCreateAccount = false;
bool CTitleUI::m_bLogin = false;
int CTitleUI::m_iLogin = -1;

CTitleUI::CTitleUI() :
	m_pIDBox(NULL),
	m_pPWBox(NULL),
	m_bButtonClicked(false),
	m_strID(NULL),
	m_strPW(NULL),
	m_pNoticeWindow(NULL),
	m_fLoginTime(0.f)
{
}

CTitleUI::CTitleUI(const CTitleUI & ui) :
	CStaticObj(ui)
{
}

CTitleUI::~CTitleUI()
{
	DestroyEdit();

	SAFE_DELETE_ARR(m_strID);
	SAFE_DELETE_ARR(m_strPW);

	Safe_Release_VecList(m_TextureList);
	Safe_Release_VecList(m_ButtonList);
	Safe_Release_Map(m_mapTab);

	SAFE_RELEASE(m_pNoticeWindow);
}

void CTitleUI::EditInit()
{
	SAFE_RELEASE(m_pIDBox);
	m_pIDBox = new CEditBox;
	m_pIDBox->CreateEditBox(LOGIN_ID_EDIT, m_ID, m_strID);

	SAFE_RELEASE(m_pPWBox);
	m_pPWBox = new CEditBox;
	m_pPWBox->CreateEditBox(LOGIN_PW_EDIT, m_PW, m_strPW, true);
}

void CTitleUI::DestroyEdit()
{
	SAFE_RELEASE(m_pIDBox);
	SAFE_RELEASE(m_pPWBox);
}

void CTitleUI::Login()
{
	if (m_pNoticeWindow)
		m_pNoticeWindow->SetWindowState(WS_CONNECTING);

	GET_SINGLE(CServerManager)->SendLogIn(m_strID, m_strPW);
}

void CTitleUI::SignUp()
{
	// 아이디, 비밀번호가 비어있으면 안내문구 띄우고 리턴
	if (strcmp(m_strID, "") == 0)
	{
		if (m_pNoticeWindow)
			m_pNoticeWindow->SetWindowState(WS_WRONGID);
		return;
	}
	else if (strcmp(m_strPW, "") == 0)
	{
		if (m_pNoticeWindow)
			m_pNoticeWindow->SetWindowState(WS_WRONGPW);
		return;
	}
	GET_SINGLE(CServerManager)->SendCreateAccount(m_strID, m_strPW);
}

void CTitleUI::Quit()
{
	GET_SINGLE(CCore)->Exit();
}

bool CTitleUI::Init()
{
	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);
	SetSize(1024.f, 768.f);

	m_ID = RECTANGLE(384.f, 330.f, 564.f, 355.f);
	m_PW = RECTANGLE(384.f, 375.f, 564.f, 400.f);

	/*m_Login = RECTANGLE(400.f, 410.f, 540.f, 440.f);
	m_SignUp = RECTANGLE(550.f, 410.f, 690.f, 440.f);
	m_Exit = RECTANGLE(700.f, 410.f, 840.f, 440.f);*/

	m_strID = new char[MAX_CHAR_LEN];
	m_strPW = new char[MAX_CHAR_LEN];

	EditInit();

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);

	// 로그인 창 배경
	SetTexture("LoginBackground", L"UI/Title/Background.png", tOrigin, tLT, POSITION(1024.f, 768.f));

	// 로그인 ui 설정
	// ui 배경
	CTexture* pUITex = AddUITexture("LoginUIBackground", L"UI/Title/Login/Background.png", POSITION(344.f, 246.f), tOrigin, tLT, POSITION(332.f, 282.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("MapleIDBackground", L"UI/Title/Login/MapleIDBack.png", POSITION(371.f, 315.f), tOrigin, tLT, POSITION(201.f, 45.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("NexonIDBackground", L"UI/Title/Login/NexonIDBack.png", POSITION(371.f, 315.f), tOrigin, tLT, POSITION(201.f, 45.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("PWBackground", L"UI/Title/Login/PWBack.png", POSITION(371.f, 361.f), tOrigin, tLT, POSITION(278.f, 44.f));
	SAFE_RELEASE(pUITex);

	CButton* pButton = AddButton("LoginButton", L"UI/Title/Login/LoginButton/", POSITION(371.f, 417.f), tOrigin, tLT, POSITION(278.f, 53.f));
	pButton->SetIconFunction(this, &CTitleUI::Login);
	SAFE_RELEASE(pButton);
	pButton = AddButton("CreateIDButton", L"UI/Title/Login/CreateIDButton/", POSITION(367.f, 477.f), tOrigin, tLT, POSITION(57.f, 21.f));
	pButton->SetIconFunction(this, &CTitleUI::SignUp);
	SAFE_RELEASE(pButton);
	pButton = AddButton("FindIDButton", L"UI/Title/Login/FindIDButton/", POSITION(431.f, 477.f), tOrigin, tLT, POSITION(50.f, 21.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("FindPWButton", L"UI/Title/Login/FindPWButton/", POSITION(485.f, 477.f), tOrigin, tLT, POSITION(54.f, 21.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("HomepageButton", L"UI/Title/Login/HomepageButton/", POSITION(547.f, 477.f), tOrigin, tLT, POSITION(56.f, 21.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("QuitButton", L"UI/Title/Login/QuitButton/", POSITION(609.f, 477.f), tOrigin, tLT, POSITION(44.f, 21.f));
	pButton->SetIconFunction(this, &CTitleUI::Quit);
	SAFE_RELEASE(pButton);
	pButton = AddButton("SaveEmailButton", L"UI/Title/Login/SaveEmailButton/", POSITION(594.f, 316.f), tOrigin, tLT, POSITION(55.f, 45.f));
	SAFE_RELEASE(pButton);

	// 탭은 일반적인 인게임 탭이랑 달라서 초기화 함수 따로 만들기
	/*CTab* pTab = AddTab("ItemTab0", L"UI/Item/Equip/", POSITION(9.f, 26.f), tOrigin, tLT, POSITION(29.f, 17.f));
	pTab->EnableTab();
	m_strLastTab = pTab->GetTag();
	m_eItemTabType = IT_EQUIP;
	SAFE_RELEASE(pTab);
	pTab = AddTab("ItemTab1", L"UI/Item/Usable/", POSITION(39.f, 28.f), tOrigin, tLT, POSITION(29.f, 17.f));
	SAFE_RELEASE(pTab);*/

	// 안내창 초기화
	m_pNoticeWindow = new CNoticeWindow;
	if (!m_pNoticeWindow->Init())
		return false;

	return true;
}

void CTitleUI::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CTitleUI::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	if (m_pIDBox)
		m_pIDBox->Update(fDeltaTime);

	if (m_pPWBox)
		m_pPWBox->Update(fDeltaTime);

	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();
	bool bMouseUp = GET_SINGLE(CMouse)->GetMouseUp();

	list<CButton*>::iterator iter;
	list<CButton*>::iterator iterEnd = m_ButtonList.end();

	for (iter = m_ButtonList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		POSITION tPos = FindPosition((*iter)->GetTag()) + m_tPos;
		(*iter)->SetIconPos(tPos);

		if ((*iter)->IconUpdate(fDeltaTime) == 1)
		{
			(*iter)->UseIcon();
		}
	}

	//unordered_map<string, CTab*>::iterator iterTab;
	//unordered_map<string, CTab*>::iterator iterTabEnd = m_mapTab.end();

	//for (iterTab = m_mapTab.begin(); iterTab != iterTabEnd; iterTab++)
	//{
	//	if (!iterTab->second->GetEnable())
	//		continue;

	//	POSITION tPos = FindPosition(iterTab->second->GetTag());
	//	iterTab->second->SetPos(tPos + m_tPos);

	//	if (iterTab->second->Update(fDeltaTime))
	//	{
	//		m_strBeforeTab = m_strLastTab;
	//		// 활성탭의 위치는 2만큼 높으므로 수정해주기
	//		tPos.y -= 2.f;
	//		SetPosition(iterTab->first, tPos);

	//		m_strLastTab = iterTab->first;
	//		m_bTabChanged = true;
	//	}
	//}

	if (m_pNoticeWindow)
	{
		switch (m_iLogin)
		{
		case 0:
			m_pNoticeWindow->SetWindowState(WS_WRONGID);
			break;
		case 1:
			m_pNoticeWindow->SetWindowState(WS_WRONGPW);
			break;
		case 2:
			m_pNoticeWindow->SetWindowState(WS_CONNECTING);
			break;
		}
		m_iLogin = -1;

		if (m_pNoticeWindow->GetEnable())
			m_pNoticeWindow->Update(fDeltaTime);
	}

	if (m_bLogin)
	{
		m_fLoginTime += fDeltaTime;

		if (m_fLoginTime >= 3.f)
		{
			m_fLoginTime = 0.f;

			if (m_pNoticeWindow)
				m_pNoticeWindow->SetEnable(false);
			
			m_bLogin = false;
			DestroyEdit();
			//printf("Game Start!\n");
			// 로그인 성공시 해상도 변경 후 렌더타겟 초기화, 그리고 텍스쳐 로딩
			GET_SINGLE(CCore)->ChangeWindowRes(1366, 768);
			// 플레이어 이름을 id로 바꿔준다
			GET_SINGLE(CSceneManager)->SetPlayerName(m_strID);
			int iLoginSceneNum = GET_SINGLE(CServerManager)->GetLoginSceneNum();
			GET_SINGLE(CSceneManager)->SetSceneToChange(iLoginSceneNum);
		}		
	}

	return 0;
}

int CTitleUI::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CTitleUI::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CTitleUI::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
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

	list<CTexture*>::iterator iterTex;
	list<CTexture*>::iterator iterTexEnd = m_TextureList.end();

	for (iterTex = m_TextureList.begin(); iterTex != iterTexEnd; iterTex++)
	{
		if (!(*iterTex)->GetEnable())
			continue;

		POSITION tPos = FindPosition((*iterTex)->GetTag()) + m_tPos;
		(*iterTex)->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	list<CButton*>::iterator iter;
	list<CButton*>::iterator iterEnd = m_ButtonList.end();

	for (iter = m_ButtonList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		POSITION tPos = FindPosition((*iter)->GetTag()) + m_tPos;
		(*iter)->SetIconPos(tPos);
		(*iter)->IconRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	unordered_map<string, CTab*>::iterator iterTab;
	unordered_map<string, CTab*>::iterator iterTabEnd = m_mapTab.end();

	for (iterTab = m_mapTab.begin(); iterTab != iterTabEnd; iterTab++)
	{
		if (!iterTab->second->GetEnable())
			continue;

		iterTab->second->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	if (m_pNoticeWindow)
	{
		if (m_pNoticeWindow->GetEnable())
			m_pNoticeWindow->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
	}

	/*HDC hDC = NULL;
	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);

	HFONT newFont, oldFont;
	GET_SINGLE(CResourcesManager)->SetFont(newFont, oldFont, hDC, 11);

	HPEN newPen = CreatePen(PS_SOLID, 1, 0);
	HPEN oldPen = (HPEN)SelectObject(hDC, newPen);

	Rectangle(hDC, (int)m_Login.l, (int)m_Login.t, (int)m_Login.r, (int)m_Login.b);
	Rectangle(hDC, (int)m_SignUp.l, (int)m_SignUp.t, (int)m_SignUp.r, (int)m_SignUp.b);
	Rectangle(hDC, (int)m_Exit.l, (int)m_Exit.t, (int)m_Exit.r, (int)m_Exit.b);
	TextOutW(hDC, (int)m_Login.l, (int)m_Login.t, L"로그인", lstrlen(L"로그인"));
	TextOutW(hDC, (int)m_SignUp.l, (int)m_SignUp.t, L"화원가입", lstrlen(L"회원가입"));
	TextOutW(hDC, (int)m_Exit.l, (int)m_Exit.t, L"종료", lstrlen(L"종료"));

	SelectObject(hDC, oldPen);
	DeleteObject(newPen);

	GET_SINGLE(CResourcesManager)->ReleaseFont(newFont, oldFont, hDC);
	pGDIRenderTarget->ReleaseDC(NULL);	*/
}

CTitleUI * CTitleUI::Clone()
{
	return new CTitleUI(*this);
}

CButton * CTitleUI::AddButton(const string & strKey, const wchar_t * pFileName, POSITION & tPos, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	CButton* pButton = new CButton;

	if (!pButton->Init())
		return NULL;

	pButton->LoadButton(strKey, pFileName, tOrigin, tLT, tRB);

	//pButton->SetUIObj(this);
	m_ButtonList.push_back(pButton);
	m_mapPos.insert(make_pair(strKey, tPos));
	pButton->AddRef();

	return pButton;
}

CTexture * CTitleUI::AddUITexture(const string & strKey, const wchar_t * pFileName, POSITION & tPos, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	_SIZE tSize = tRB - tLT;
	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, tOrigin, tSize, tLT, tRB);

	if (!pTex)
		return NULL;

	m_TextureList.push_back(pTex);
	SetPosition(strKey, tPos);
	pTex->AddRef();

	return pTex;
}

CTab * CTitleUI::AddTab(const string & strKey, const wchar_t * pFileName, POSITION & tPos, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	CTab* pTab = new CTab;

	if (!pTab->Init())
		return NULL;

	pTab->LoadTab(strKey, pFileName, tOrigin, tLT, tRB);

	m_mapTab.insert(make_pair(strKey, pTab));
	m_mapPos.insert(make_pair(strKey, tPos));
	pTab->AddRef();

	return pTab;
}

void CTitleUI::SetPosition(const string & strKey, POSITION & tPos)
{
	map<string, POSITION>::iterator iter = m_mapPos.find(strKey);

	if (iter == m_mapPos.end())
		m_mapPos.insert(make_pair(strKey, tPos));
	else
		iter->second = tPos;
}

POSITION CTitleUI::FindPosition(const string & strKey)
{
	map<string, POSITION>::iterator iter = m_mapPos.find(strKey);

	if (iter == m_mapPos.end())
		return POSITION(0.f, 0.f);

	return iter->second;
}
