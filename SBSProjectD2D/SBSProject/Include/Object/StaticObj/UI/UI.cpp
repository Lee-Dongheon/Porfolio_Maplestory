#include "UI.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../Stage/Stage.h"
#include "../../../Core/Input.h"
#include "../../../Scene/Layer.h"

map<UI_TYPE, CUI*> CUI::m_mapUI;
UI_TYPE CUI::m_eLastUIType;
map<string, CIcon*> CUI::m_mapIcon; // 키세팅에 들어가있는 아이콘 리스트
map<string, CTexture*> CUI::m_mapKeyImage;
CIcon* CUI::m_pSelectedIcon;
bool CUI::m_bHaveSelectedIcon;

CUI::CUI() :
	m_bButtonClicked(false),
	m_iZOrder(1),
	m_pPlayer(NULL),
	m_bIsSelected(false),
	m_bTabChanged(false)
{
}

CUI::CUI(const CUI& ui) :
	CStaticObj(ui)
{
	m_bButtonClicked = false;
	m_iZOrder = 1;
	m_pPlayer = NULL;
	m_bIsSelected = false;
	m_bTabChanged = false;
	m_tHeadSize = ui.m_tHeadSize;
	m_strSelectedTab = ui.m_strSelectedTab;
}

CUI::~CUI()
{
	Safe_Release_VecList(m_ButtonList);
	Safe_Release_VecList(m_UITextureList);
	Safe_Release_Map(m_mapTab);
	Safe_Release_Map(m_mapIcon);
	Safe_Release_Map(m_mapKeyImage);
	SAFE_RELEASE(m_pSelectedIcon);
	SAFE_RELEASE(m_pPlayer);
}

void CUI::SetPlayer(CObj * pPlayer)
{
	SAFE_RELEASE(m_pPlayer);
	m_pPlayer = (CPlayer*)pPlayer;
	m_pPlayer->AddRef();
}

CUI * CUI::FindUI(UI_TYPE eType)
{
	map<UI_TYPE, CUI*>::iterator iter = m_mapUI.find(eType);

	if (iter == m_mapUI.end())
		return NULL;
	
	return iter->second;
}

CUI * CUI::FindNextUI(UI_TYPE eType)
{
	map<UI_TYPE, CUI*>::iterator iter;
	map<UI_TYPE, CUI*>::iterator iterEnd = m_mapUI.end();

	for (iter = m_mapUI.begin(); iter != iterEnd; iter++)
	{
		if (iter->second->GetBeforeUIType() == eType)
			return iter->second;
	}

	return NULL;	
}

void CUI::ReleasePlayer()
{
	SAFE_RELEASE(m_pPlayer);
}

bool CUI::Init(UI_TYPE eType)
{
	m_UITextureList.clear();
	m_ButtonList.clear();
	m_mapTab.clear();
	m_mapPos.clear();

	m_eUIType = eType;
	m_eBeforeUIType = UI_DEFAULT;

	m_mapUI.insert(make_pair(eType, this));

	// 갱신을 위함
	m_bTabChanged = true;
	
	return true;
}

void CUI::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);

	list<CButton*>::iterator iter;
	list<CButton*>::iterator iterEnd = m_ButtonList.end();

	for (iter = m_ButtonList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		(*iter)->Input(fDeltaTime);
	}

	unordered_map<string, CTab*>::iterator iterTab;
	unordered_map<string, CTab*>::iterator iterTabEnd = m_mapTab.end();

	for (iterTab = m_mapTab.begin(); iterTab != iterTabEnd; iterTab++)
	{
		if (!iterTab->second->GetEnable())
			continue;

		iterTab->second->Input(fDeltaTime);
	}

	// 활성창에서 tab을 누르는 경우
	if (KEYPRESS("Tab") && m_bIsSelected)
	{
		// 현재 tab 이름으로 map iterator 가져오기
		m_strBeforeTab = m_strLastTab;
		unordered_map<string, CTab*>::iterator iterTemp = m_mapTab.find(m_strLastTab);
		if (iterTemp == m_mapTab.end())
			return;
		// iterator 한 칸 다음으로
		iterTemp++;
		// 만약 마지막이었다면 처음 tab으로
		if (iterTemp == m_mapTab.end())
			iterTemp = m_mapTab.begin();
		// 활성탭의 위치는 2만큼 높으므로 수정해주기
		POSITION tPos = FindPosition(iterTemp->first);
		tPos.y -= 2.f;
		SetPosition(iterTemp->first, tPos);

		m_strLastTab = iterTemp->first;
		iterTemp->second->EnableTab();
		m_bTabChanged = true;
	}
}

int CUI::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);
	
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();
	RECTANGLE tRect = RECTANGLE(m_tPos.x, m_tPos.y,
		m_tPos.x + m_tHeadSize.x, m_tPos.y + m_tHeadSize.y);

	// 마우스 변화 거리 반영
	if (m_bIsSelected && bIsClicked && PointInRect(tRect, tMousePos))
	{
		POSITION tMoveDist = GET_SINGLE(CMouse)->GetMoveDist();
		m_tPos += tMoveDist;
	}

	list<CButton*>::iterator iter;
	list<CButton*>::iterator iterEnd = m_ButtonList.end();

	for (iter = m_ButtonList.begin(); iter != iterEnd; iter++)
	{
		if (!(*iter)->GetEnable())
			continue;

		POSITION tPos = FindPosition((*iter)->GetTag()) + m_tPos;
		(*iter)->SetIconPos(tPos);

		if((*iter)->IconUpdate(fDeltaTime) == 1)
		{
			(*iter)->UseIcon();
		}
	}

	unordered_map<string, CTab*>::iterator iterTab;
	unordered_map<string, CTab*>::iterator iterTabEnd = m_mapTab.end();

	for (iterTab = m_mapTab.begin(); iterTab != iterTabEnd; iterTab++)
	{
		if (!iterTab->second->GetEnable())
			continue;

		POSITION tPos = FindPosition(iterTab->second->GetTag());
		iterTab->second->SetPos(tPos + m_tPos);

		if (iterTab->second->Update(fDeltaTime))
		{
			m_strBeforeTab = m_strLastTab;
			// 활성탭의 위치는 2만큼 높으므로 수정해주기
			tPos.y -= 2.f;
			SetPosition(iterTab->first, tPos);

			m_strLastTab = iterTab->first;
			m_bTabChanged = true;
		}			
	}

	return 0;
}

int CUI::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);

	POSITION tLastTabPos = FindPosition(m_strLastTab);

	unordered_map<string, CTab*>::iterator iterTab;
	unordered_map<string, CTab*>::iterator iterTabEnd = m_mapTab.end();

	for (iterTab = m_mapTab.begin(); iterTab != iterTabEnd; iterTab++)
	{
		if (!iterTab->second->GetEnable())
			continue;

		if (m_bTabChanged && iterTab->first == m_strBeforeTab)
		{
			// 비활성탭의 높이는 활성탭의 높이보다 2 작으므로 수정
			POSITION tPos = FindPosition(iterTab->first);
			POSITION tempPos = POSITION(tPos.x, tLastTabPos.y + 2.f);

			SetPosition(iterTab->first, tempPos);
			iterTab->second->DisableTab();
		}

		iterTab->second->LateUpdate(fDeltaTime);
	}
	m_bTabChanged = false;

	return 0;
}

void CUI::Collision(float fDeltaTime)
{
}

void CUI::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	list<CTexture*>::iterator iterTex;
	list<CTexture*>::iterator iterTexEnd = m_UITextureList.end();

	for (iterTex = m_UITextureList.begin(); iterTex != iterTexEnd; iterTex++)
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
}

void CUI::UseUIIcon()
{
	if (m_bEnable)
		CloseUI();
	else
		OpenUI();
}

void CUI::SetZOrder(int iZOrder)
{
	if (m_eUIType == UI_MAIN)
		return;

	m_iZOrder = iZOrder;

	if (iZOrder == INT_MAX)
	{
		m_bIsSelected = true;
		
		// 자신의 eType 을 before로 가지고 있던 객체 찾기
		// 즉 자신 다음으로 켜졌던 객체
		CUI* pUI = FindNextUI(m_eUIType);
		if (pUI) // 켜져 있다면 해당 객체에게 나의 before type을 건네주기
		{
			if (pUI->GetEnable())
				pUI->SetBeforeUIType(m_eBeforeUIType);
		}
		// 마지막에 켜졌던 type을 자신의 before로 가져오기
		// 단 자신이 last로 지정되어 있을 경우엔 하지말자.
		if (m_eLastUIType != m_eUIType)
			m_eBeforeUIType = m_eLastUIType;
	}		
	else
		m_bIsSelected = false;

}

void CUI::OpenUI()
{
	SetEnable(true);
	SetZOrder(INT_MAX);

	map<UI_TYPE, CUI*>::iterator iter;
	map<UI_TYPE, CUI*>::iterator iterEnd = m_mapUI.end();

	for (iter = m_mapUI.begin(); iter != iterEnd; iter++)
	{
		if (iter->second == this)
			continue;
		iter->second->SetZOrder(1);
	}
	// 개인 before type 에 static last type 대입
	m_eBeforeUIType = m_eLastUIType;
	// static last type 에 자신의 type 대입
	m_eLastUIType = m_eUIType;
}

void CUI::CloseUI()
{
	if (m_eUIType == UI_MAIN)
	{
		// Setting 켜주기

		return;
	}

	SetEnable(false);
	SetZOrder(1);

	// 자신이 가장 마지막에 켜졌다면
	if (m_eLastUIType == m_eUIType)
	{
		// before ui를 last ui로 만들어주자.
		// 대신 before가 main이라면 zorder는 건드리지 말자.
		map<UI_TYPE, CUI*>::iterator iter = m_mapUI.find(m_eBeforeUIType);
		if (iter == m_mapUI.end())
			return;
		m_eLastUIType = iter->first;
		
		if (iter->first != UI_MAIN)
			iter->second->SetZOrder(INT_MAX);
	}
	else // 자신은 중간에 켜졌던 것이라면
	{
		// 자신의 eType 을 before로 가지고 있던 객체 찾기
		// 즉 자신 다음으로 켜졌던 객체
		CUI* pUI = FindNextUI(m_eUIType);
		if (pUI) // 켜져 있다면 해당 객체에게 나의 before type을 건네주기
		{
			if (pUI->GetEnable())
				pUI->SetBeforeUIType(m_eBeforeUIType);
		}
	}

	// 끄면서 before type을 default로 설정
	m_eBeforeUIType = UI_DEFAULT;
}

CButton* CUI::FindButton(const string & strTag)
{
	list<CButton*>::iterator iter;
	list<CButton*>::iterator iterEnd = m_ButtonList.end();

	for (iter = m_ButtonList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return (*iter);
		}			
	}

	return NULL;
}

CTexture* CUI::FindUITexture(const string & strTag)
{
	list<CTexture*>::iterator iter;
	list<CTexture*>::iterator iterEnd = m_UITextureList.end();

	for (iter = m_UITextureList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return (*iter);
		}			
	}

	return NULL;
}

CIcon * CUI::FindIcon(const string & strTag)
{
	map<string, CIcon*>::iterator iter = m_mapIcon.find(strTag);

	if (iter == m_mapIcon.end())
		return NULL;

	iter->second->AddRef();
	return iter->second;
}

string CUI::FindIconKey(CIcon * pIcon)
{
	map<string, CIcon*>::iterator iter;
	map<string, CIcon*>::iterator iterEnd = m_mapIcon.end();

	for (iter = m_mapIcon.begin(); iter != iterEnd; iter++)
	{
		if (iter->second == pIcon)
			return iter->first;
	}

	return "";
}

bool CUI::FindIcon(CIcon * pIcon)
{
	map<string, CIcon*>::iterator iter;
	map<string, CIcon*>::iterator iterEnd = m_mapIcon.end();

	for (iter = m_mapIcon.begin(); iter != iterEnd; iter++)
	{
		if (iter->second == pIcon)
			return true;
	}

	return false;
}

CTexture * CUI::FindKeyImage(const string & strTag)
{
	map<string, CTexture*>::iterator iter = m_mapKeyImage.find(strTag);

	if (iter == m_mapKeyImage.end())
		return NULL;

	iter->second->AddRef();
	return iter->second;
}

void CUI::AddIcon(const string & strTag, CIcon * pIcon)
{
	// 아이콘을 넣는 경우는 많지만 크게 두가지
	// 없던 아이콘을 새로 추가하는 경우
	// 있는 아이콘 위치를 바꾸는 경우

	// 넣을 아이콘이 맵 안에 이미 존재하는지 체크하여 위치 정보 가지고 있기.
	string strBeforeTag = "";
	if (FindIcon(pIcon))
		strBeforeTag = FindIconKey(pIcon);

	// 아이콘을 넣기전에 넣을 위치의 아이콘 찾아보자.
	CIcon* pBeforeIcon = FindIcon(strTag);
	// 넣을 자리에 아이콘이 존재했으면
	if (pBeforeIcon)
	{
		// keyIcon이라면 default로 보내주자.
		if (pBeforeIcon->GetIconType() == IC_KEY)
		{
			AddIcon(((CKeyIcon*)pBeforeIcon)->GetDefaultKey(), pBeforeIcon);
			// 여기 재귀라서 무한 호출이 되지 않도록 조심해야한다.
			// default자리에는 아이콘추가를 무시하도록 추가로 짜주자.
		}
		// 스킬, 아이템은 상관없음
		// 그러고 나서 현재 자리는 지워주자.
		EraseIcon(strTag);
		SAFE_RELEASE(pBeforeIcon);
	}
	
	// 만약 여기서 아이콘 존재여부를 찾게 되면
	// 이미 넣고난 뒤이므로 무조건 존재하게 된다
	// 따라서 함수를 시작할때 미리 가지고 있게 설계하였다.

	// 기존에 존재하던 아이콘 처리 후엔 넣어주면 된다.
	// 태그가 올바르면!
	if (strTag != "")
	{
		pIcon->AddRef();
		m_mapIcon.insert(make_pair(strTag, pIcon));
	}
		
	// 그리고 이 아이콘이 원래 맵에 존재했었다면,
	// 그 키값에 있던 아이콘 없애주기
	if (strBeforeTag != "")
		EraseIcon(strBeforeTag);
}

void CUI::EraseIcon(const string & strTag)
{
	map<string, CIcon*>::iterator iter = m_mapIcon.find(strTag);

	if (iter == m_mapIcon.end())
		return;

	SAFE_RELEASE(iter->second);
	m_mapIcon.erase(strTag);
}

CButton* CUI::AddButton(const string & strKey, const wchar_t * pFileName, POSITION& tPos, POSITION& tOrigin, POSITION& tLT, POSITION& tRB)
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
CTexture* CUI::AddUITexture(const string & strKey, const wchar_t * pFileName, POSITION& tPos, POSITION& tOrigin, POSITION& tLT, POSITION& tRB)
{
	_SIZE tSize = tRB - tLT;
	CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, tOrigin, tSize, tLT, tRB);

	if (!pTex)
		return NULL;

	m_UITextureList.push_back(pTex);
	SetPosition(strKey, tPos);
	pTex->AddRef();

	return pTex;
}
CTab * CUI::AddTab(const string & strKey, const wchar_t * pFileName, POSITION & tPos, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
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
CKeyIcon * CUI::AddKeyIcon(const string & strTag, const wchar_t * pFileName, const string& strKey)
{
	CKeyIcon* pKeyIcon = new CKeyIcon;

	if (!pKeyIcon->Init())
		return NULL;

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);
	POSITION tRB = POSITION(32.f, 32.f);

	pKeyIcon->LoadTextureIcon(strTag, pFileName, tOrigin, tLT, tRB);
	
	//pKeyIcon->SetUIObj(this);
	m_mapIcon.insert(make_pair(strKey, pKeyIcon));
	pKeyIcon->AddRef();

	return pKeyIcon;
}
void CUI::SetPosition(const string & strKey, POSITION & tPos)
{
	map<string, POSITION>::iterator iter = m_mapPos.find(strKey);

	if (iter == m_mapPos.end())
		m_mapPos.insert(make_pair(strKey, tPos));
	else
		iter->second = tPos;
}
POSITION CUI::FindPosition(const string & strKey)
{
	map<string, POSITION>::iterator iter = m_mapPos.find(strKey);

	if (iter == m_mapPos.end())
		return POSITION(0.f, 0.f);

	return iter->second;
}