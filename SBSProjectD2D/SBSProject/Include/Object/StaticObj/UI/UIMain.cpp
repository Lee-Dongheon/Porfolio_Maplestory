#include "UIMain.h"
#include "../../../Core/Core.h"
#include "../../../Core/Mouse.h"
#include "../../../Math.h"
#include "../../../Core/Input.h"
#include "../EditBox/EditBox.h"
#include "../../../Core/ServerManager.h"

//#include "../../StaticObj/Icon/Item/Item.h"
//#include "../../StaticObj/Icon/Skill/Skill.h"

CUIMain::CUIMain() :
	m_bQuickExtend(false),
	m_bQuickChange(false),
	m_fQuickChangeTime(0.f),
	m_fQuickChangeLimitTime(0.2f),
	m_fQuickFoldPosX(0.f),
	m_fQuickExtendPosX(0.f),
	m_pChatEditBox(NULL),
	m_strMessageBuffer(NULL),
	m_iLastMessage(0)
{
	for (int i = 0; i < 5; i++)
	{
		m_strMessages[i] = NULL;
	}
}

CUIMain::CUIMain(const CUIMain & ui) :
	CUI(ui)
{
	m_bQuickExtend = false;
	m_bQuickChange = false;
	m_fQuickChangeTime = ui.m_fQuickChangeTime;
	m_fQuickChangeLimitTime = ui.m_fQuickChangeLimitTime;

	m_fQuickFoldPosX = ui.m_fQuickFoldPosX;
	m_fQuickExtendPosX = ui.m_fQuickExtendPosX;
}

CUIMain::~CUIMain()
{
	for (int i = 0; i < 5; i++)
	{
		SAFE_DELETE_ARR(m_strMessages[i]);
	}
	SAFE_RELEASE(m_pChatEditBox);
	SAFE_DELETE_ARR(m_strMessageBuffer);
	Safe_Release_Map(m_mapNum);
	Safe_Release_Map(m_mapLevelNum);
}

void CUIMain::RecvMessage(PPACKETMESSAGE pPacket)
{
	memset(m_strMessages[m_iLastMessage], '\0', MAX_CHAR_LEN + MAX_MESSAGE_LEN + 3);
	sprintf(m_strMessages[m_iLastMessage], "%s : %s", pPacket->name, pPacket->message);
	m_iLastMessage++;
	if (m_iLastMessage >= 5)
		m_iLastMessage = 0;
}

CTexture * CUIMain::FindNum(int i)
{
	map<int, CTexture*>::iterator iter = m_mapNum.find(i);

	if (iter == m_mapNum.end())
		return NULL;

	// private이므로 ref 안할것
	//iter->second->AddRef();
	return iter->second;
}

CTexture * CUIMain::FindLevelNum(int i)
{
	map<int, CTexture*>::iterator iter = m_mapLevelNum.find(i);

	if (iter == m_mapLevelNum.end())
		return NULL;

	// private이므로 ref 안할것
	//iter->second->AddRef();
	return iter->second;
}

bool CUIMain::Init()
{
	if (!CUI::Init(UI_MAIN))
		return false;

	SetPos(0.f, 0.f);
	SetSize(1366.f, 768.f);
	SetHeadSize(0.f, 0.f);

	m_eLastUIType = UI_MAIN;

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);
	// tRB 는 size와 동일하게

	CButton* pButton = AddButton("CashShop", L"UI/UImain/CashShop/", POSITION(794.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Event", L"UI/UImain/Event/", POSITION(829.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Character", L"UI/UImain/Character/", POSITION(864.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Community", L"UI/UImain/Community/", POSITION(899.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	SAFE_RELEASE(pButton);
	pButton = AddButton("Setting", L"UI/UImain/Setting/", POSITION(934.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	pButton->SetIconFunction(this, &CUIMain::Exit);
	SAFE_RELEASE(pButton);
	pButton = AddButton("Menu", L"UI/UImain/Menu/", POSITION(969.f, 720.f), tOrigin, tLT, POSITION(34.f, 37.f));
	SAFE_RELEASE(pButton);

	pButton = AddButton("QuickExtend", L"UI/UImain/quickExtend/", POSITION(1007.f, 686.f), tOrigin, tLT, POSITION(13.f, 71.f));
	pButton->SetIconFunction(this, &CUIMain::QuickSlot);
	SAFE_RELEASE(pButton);
	pButton = AddButton("QuickFold", L"UI/UImain/quickFold/", POSITION(1007.f, 686.f), tOrigin, tLT, POSITION(13.f, 71.f));
	pButton->SetIconFunction(this, &CUIMain::QuickSlot);
	pButton->SetEnable(false);
	SAFE_RELEASE(pButton);

	CTexture* pUITex = AddUITexture("StatusBackground", L"UI/UImain/status_background.png", POSITION(588.f, 714.f), tOrigin, tLT, POSITION(198.f, 37.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("StatusHPBar", L"UI/UImain/status_gauge_hp.png", POSITION(610.f, 716.f), tOrigin, tLT, POSITION(171.f, 13.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("StatusMPBar", L"UI/UImain/status_gauge_mp.png", POSITION(610.f, 732.f), tOrigin, tLT, POSITION(171.f, 13.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("StatusCover", L"UI/UImain/statusCover.png", POSITION(585.f, 688.f), tOrigin, tLT, POSITION(204.f, 70.f));
	SAFE_RELEASE(pUITex);
	
	KeyImageInit();
	QuickSlotInit();

	m_fQuickFoldPosX = 1017.f;
	m_fQuickExtendPosX = 807.f;	

	pUITex = AddUITexture("QuickBack", L"UI/UImain/quickSlot_background.png", POSITION(1017.f, 688.f), tOrigin, tLT, POSITION(557.f, 67.f));
	SAFE_RELEASE(pUITex);
	pUITex = AddUITexture("QuickCover", L"UI/UImain/quickSlot_cover.png", POSITION(1015.f, 685.f), tOrigin, tLT, POSITION(562.f, 73.f));
	SAFE_RELEASE(pUITex);

	AddNums("Num", L"UI/UImain/ExpNum", tOrigin, tLT, POSITION(7.f, 9.f));
	AddNums("LevelNum", L"UI/UImain/LevelNum", tOrigin, tLT, POSITION(7.f, 9.f), true);

	CKeyIcon* pIcon = AddKeyIcon("QuickIcon", L"UI/Key/Icon_QuickSlot.png", "]");
	pIcon->SetDefaultKey("Extra2");
	pIcon->SetIconFunction(this, &CUIMain::QuickSlot);
	SAFE_RELEASE(pIcon);
	pIcon = AddKeyIcon("SettingIcon", L"UI/Key/Icon_Setting.png", "ESC");
	pIcon->SetDefaultKey("ESC");
	pIcon->SetIconFunction(this, &CUIMain::Setting);
	SAFE_RELEASE(pIcon);

	m_tMessageBox = RECTANGLE(0.f, 635.f, 300.f, 735.f);
	m_tChatBox = RECTANGLE(0.f, 737.f, 278.f, 757.f);
	m_tSendButton = RECTANGLE(280.f, 737.f, 300.f, 757.f);
	
	// 메세지 버퍼
	m_strMessageBuffer = new char[MAX_MESSAGE_LEN];
	memset(m_strMessageBuffer, '\0', MAX_MESSAGE_LEN);

	/*m_pChatEditBox = new CEditBox;
	m_pChatEditBox->CreateEditBox(200, m_tChatBox, m_strMessageBuffer);*/

	// 받는 메세지 버퍼
	for (int i = 0; i < 5; i++)
	{
		// 이름 : 메세지 형식으로 크기 할당
		m_strMessages[i] = new char[MAX_MESSAGE_LEN + MAX_CHAR_LEN + 3];
		memset(m_strMessages[i], '\0', MAX_MESSAGE_LEN + MAX_CHAR_LEN + 3);
	}

	return true;
}

void CUIMain::Input(float fDeltaTime)
{
	CUI::Input(fDeltaTime);

	if (m_pPlayer)
	{
		// 채팅모드시 입력받지 않음
		if (m_pPlayer->m_bChatMode)
			return;
	}

	vector<string> vecKeyPress = GETKEYPRESSVEC();

	for (int i = 0; i < (int)vecKeyPress.size(); i++)
	{
		map<string, CIcon*>::iterator iter = m_mapIcon.find(vecKeyPress[i]);
		if (iter == m_mapIcon.end())
			continue;

		iter->second->UseIcon();
	}

	if (m_pChatEditBox)
		m_pChatEditBox->Input(fDeltaTime);
}

int CUIMain::Update(float fDeltaTime)
{
	CUI::Update(fDeltaTime);

	if (m_bQuickChange)
	{
		m_fQuickChangeTime += fDeltaTime;
		// (접은 위치 - 편 위치)(= 총 변화량) / 총 시간 * 현재시간 = 시간당 변화량(양수)
		float fGap = (m_fQuickFoldPosX - m_fQuickExtendPosX) / m_fQuickChangeLimitTime * fDeltaTime;
		// 펴는중이면 변화량이 음수
		if (m_bQuickExtend) // 펴는 상태
			fGap = -fGap;

		// 위치 기준인 background 위치 불러오기
		POSITION tPos = FindPosition("QuickBack");
		// 위치 보정
		tPos.x += fGap;
		if (807.f > tPos.x && m_bQuickExtend) // 펴는중에 807보다 작아지면 보정
			tPos.x = 807.f;
		if (1017.f < tPos.x && !m_bQuickExtend) // 접는중에 1017보다 커지면 보정
			tPos.x = 1017.f;
		SetPosition("QuickBack", tPos);
		// 연관된 다른 버튼/ui 위치도 변경
		SetPosition("QuickCover", POSITION(tPos.x - 2.f, tPos.y - 3.f));
		SetPosition("QuickExtend", POSITION(tPos.x - 10.f, tPos.y - 2.f));
		SetPosition("QuickFold", POSITION(tPos.x - 10.f, tPos.y - 2.f));

		// 퀵슬롯 변화와 함께 숨겨야 하는 버튼들
		fGap = 60.f / m_fQuickChangeLimitTime * fDeltaTime;
		// 접는중이면 음수
		if (!m_bQuickExtend) // 접는 상태
			fGap = -fGap;

		// 위치 기준 Cashshop 버튼, 각 간격 35.f
		tPos = FindPosition("CashShop");
		tPos.y += fGap;
		if (720.f > tPos.y && !m_bQuickExtend) // 접는중에 720보다 작아지면 보정
			tPos.y = 720.f;
		if (790.f < tPos.y && m_bQuickExtend) // 펴는중에 790보다 커지면 보정
			tPos.y = 780.f;
		SetPosition("CashShop", tPos);
		// 연관된 나머지 버튼도 변경
		SetPosition("Event", POSITION(tPos.x + 35.f * 1.f, tPos.y));
		SetPosition("Character", POSITION(tPos.x + 35.f * 2.f, tPos.y));
		SetPosition("Community", POSITION(tPos.x + 35.f * 3.f, tPos.y));
		SetPosition("Setting", POSITION(tPos.x + 35.f * 4.f, tPos.y));
		SetPosition("Menu", POSITION(tPos.x + 35.f * 5.f, tPos.y));
	}
	
	POSITION tPos = FindPosition("QuickBack");

	map<POSITION, string>::iterator iter;
	map<POSITION, string>::iterator iterEnd = m_mapQuickPos.end();

	for (iter = m_mapQuickPos.begin(); iter != iterEnd; iter++)
	{
		CIcon* pIcon = FindIcon(iter->second);
		if (pIcon)
		{
			pIcon->SetQuickPos(tPos + POSITION(iter->first.x*35.f, iter->first.y *35.f));

			// 눌렀다면
			if (pIcon->QuickUpdate(fDeltaTime) && !m_pSelectedIcon && !m_bHaveSelectedIcon)
			{
				m_pSelectedIcon = pIcon;
				m_pSelectedIcon->AddRef();
			}
			SAFE_RELEASE(pIcon);
		}
	}

	// 퀵슬롯이 아닌 다른 위치를 누르면 아이콘 그냥 소멸시켜야함
	// 하지만 main이 uilayer에서 가장먼저 업데이트 되므로
	// 여기서 지우면 다른 ui에서 m_pSelectedIcon을 처리하지 못함
	// 따라서 아이콘 소멸은 lateupdate에서 처리하자.

	// 마우스의 위치부터 체크하자.
	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	// 퀵슬롯 위치는 tPos(위에 이미 찾아서 갖다씀
	RECTANGLE tRect = RECTANGLE(tPos.x, tPos.y, tPos.x + 557.f, tPos.y + 67.f);
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();

	if (m_pSelectedIcon && m_bHaveSelectedIcon && bIsClicked)
	{ // 퀵슬롯 창은 아이콘 타입에 제한이 없다.
	  // 퀵슬롯 창 안을 눌렀는지 체크, 안이라면 icon의 quickPos 변화
		if (PointInRect(tRect, tMousePos))
		{
			// 마우스 위치의 퀵슬롯 tag 가져오기
			string strTag = FindQuickTag(tMousePos);
			// 해당 태그에 아이콘이 존재하는가?
			CIcon* pIcon = FindIcon(strTag);
			// 존재한다면
			if (pIcon)
			{
				// 우선 가지고 있던것 넣어주고
				AddIcon(strTag, m_pSelectedIcon);
				// skill 이나 item은 이미 다른 ui가 갖고 있으므로
				// AddIcon과정에서 release를 해도 없어지지 않음(키세팅에서 없어져도 존재함)
				// keyIcon은 AddIcon 과정에서 default로 옮겨지고
				// keyIcon은 release하지 않음.
				// 따라서 pIcon은 사라지지 않는다. (temp에 옮길 생각했으나 필요 X)

				// 원래 있던 아이콘을 마우스로 쥐어준다.
				SAFE_RELEASE(m_pSelectedIcon);
				m_pSelectedIcon = pIcon;
				SAFE_RELEASE(pIcon);
				m_pSelectedIcon->AddRef();
				// 플래그는 false로 만들어줘야 중복호출이 안된다.				
				m_bHaveSelectedIcon = false;
			}
			else // 아니라면 그냥 넣자.
			{
				AddIcon(strTag, m_pSelectedIcon);
				SAFE_RELEASE(m_pSelectedIcon);
			}			
		}
	}

	bool bMouseUp = GET_SINGLE(CMouse)->GetMouseUp();
	//if (m_pPlayer)
	//{
	//	if (m_pPlayer->m_bChatMode && !m_pChatEditBox)
	//	{
	//		// 채팅모드 활성화되면 chatbox 생성
	//		m_pChatEditBox->CreateEditBox(200, m_tChatBox, m_strMessageBuffer);
	//	}
	//}

	// 채팅박스 누르면 chatbox 생성
	if (PointInRect(m_tChatBox, tMousePos) && bMouseUp && !m_pChatEditBox)
	{
		m_pChatEditBox = new CEditBox;
		m_pChatEditBox->CreateEditBox(200, m_tChatBox, m_strMessageBuffer);
		m_pPlayer->m_bChatMode = true;
	}

	if (m_pChatEditBox)
		m_pChatEditBox->Update(fDeltaTime);

	// 보내기 버튼
	if (PointInRect(m_tSendButton, tMousePos) && bMouseUp)
	{
		// 메세지 보내기
		GET_SINGLE(CServerManager)->SendMessagePacket(m_pPlayer->GetTag().c_str(), m_strMessageBuffer);
		// 보내면 채팅 박스 초기화
		SAFE_RELEASE(m_pChatEditBox);
		m_pPlayer->m_bChatMode = false;
	}

	return 0;
}

int CUIMain::LateUpdate(float fDeltaTime)
{
	CUI::LateUpdate(fDeltaTime);

	POSITION tMousePos = GET_SINGLE(CMouse)->GetPos();
	bool bIsClicked = GET_SINGLE(CMouse)->GetClicked();
	bool bMouseUp = GET_SINGLE(CMouse)->GetMouseUp();

	if (m_bQuickChange)
	{
		// 변화 시간이 지났으면 변화 종료
		if (m_fQuickChangeTime >= m_fQuickChangeLimitTime)
		{
			m_bQuickChange = false;
			m_fQuickChangeTime = 0.f;
			// 활성화 상태 반대로 바꿔주기
			CButton* pButton = FindButton("QuickExtend");
			pButton->SetEnable(!pButton->GetEnable());
			SAFE_RELEASE(pButton);
			pButton = FindButton("QuickFold");
			pButton->SetEnable(!pButton->GetEnable());
			SAFE_RELEASE(pButton);
		}
	}

	// 다른 ui들의 update에서 m_pSelectedIcon이 처리되지 않고
	// main의 lateupdate에서 처리된다면
	// 허공에 클릭한 것이므로 그냥 release 해주자.
	if (m_pSelectedIcon && m_bHaveSelectedIcon && bIsClicked)
		SAFE_RELEASE(m_pSelectedIcon);

	// 처음 눌렀다 떼면 가지고있다가
	// 다시 누르면 사라지고 떼면 플래그를 되돌려준다.
	// 총 네 가지 상황이 순환 :	m_pSelectedIcon	|	m_bHaveSelectedIcon
	// 1) 아이콘 없을때 클릭			T					F
	// 2) 아이콘 생기고 뗌				T					T
	// 3) 아이콘 있을때 클릭			F					T
	// 4) 아이콘 없어지고 뗌			F					F
	// 1), 3) 상황에서 각각 m_bHaveSelectedIcon 플래그를 바꿔주는 역할
	// 각 ui에서 4) 상황에 1)로 변환, 2) 상황에서 3)으로 변환 을 처리해줌
	
	// m_pSelectedIcon은 static 변수이므로 플래그 변화는 main에서만 처리해주기
	if (bMouseUp)
	{
		if (m_pSelectedIcon && !m_bHaveSelectedIcon)
			m_bHaveSelectedIcon = true;
		if (!m_pSelectedIcon && m_bHaveSelectedIcon)
			m_bHaveSelectedIcon = false;
	}

	return 0;
}

void CUIMain::Collision(float fDeltaTime)
{
	CUI::Collision(fDeltaTime);
}

void CUIMain::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CUI::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	if (m_pPlayer)
	{
		int iLevel = m_pPlayer->GetLevel();
		if (iLevel / 100 > 0)
		{
			int temp = iLevel / 100;
			
			CTexture* pLvTex = FindLevelNum(temp);
			if (pLvTex)
			{
				POSITION tPos = POSITION(628.f, 696.f);
				pLvTex->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
			}
		}
		if (iLevel / 10 > 0)
		{
			int temp = (iLevel / 10) % 10;

			CTexture* pLvTex = FindLevelNum(temp);
			if (pLvTex)
			{
				POSITION tPos = POSITION(636.f, 696.f);
				pLvTex->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
			}
		}
		int temp = iLevel % 10;
		CTexture* pLvTex = FindLevelNum(temp);
		if (pLvTex)
		{
			POSITION tPos = POSITION(644.f, 696.f);
			pLvTex->Render(tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
		}
		HDC hDC = NULL;
		pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
		HFONT newFont, oldFont;
		GET_SINGLE(CResourcesManager)->SetFont(newFont, oldFont, hDC, 11);
		const char* pPlayerName = m_pPlayer->GetTag().c_str();
		TextOutA(hDC, 699, 696, pPlayerName, strlen(pPlayerName));
		GET_SINGLE(CResourcesManager)->ReleaseFont(newFont, oldFont, hDC);
		pGDIRenderTarget->ReleaseDC(NULL);
	}
	
	POSITION tPos = FindPosition("QuickBack");

	map<POSITION, string>::iterator iter;
	map<POSITION, string>::iterator iterEnd = m_mapQuickPos.end();

	for (iter = m_mapQuickPos.begin(); iter != iterEnd; iter++)
	{
		CIcon* pIcon = FindIcon(iter->second);
		if (pIcon)
		{
			pIcon->QuickRender(pGDIRenderTarget, pRenderTarget, fDeltaTime);				
			SAFE_RELEASE(pIcon);
		}

		CTexture* pKeyImage = FindKeyImage(iter->second);
		if (pKeyImage)
		{
			POSITION tempPos = tPos + POSITION(iter->first.x * 35.f, iter->first.y * 35.f) + POSITION(2.f, 3.f);
			pKeyImage->Render(tempPos, pGDIRenderTarget, pRenderTarget, fDeltaTime);
			SAFE_RELEASE(pKeyImage);
		}
	}

	HDC hDC;
	pGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDC);
	SetBkMode(hDC, TRANSPARENT);

	int temp = m_iLastMessage + 1;
	if (temp >= 5)
		temp = 0;

	Rectangle(hDC, m_tMessageBox.l, m_tMessageBox.t, m_tMessageBox.r, m_tMessageBox.b);
	Rectangle(hDC, m_tChatBox.l, m_tChatBox.t, m_tChatBox.r, m_tChatBox.b);
	Rectangle(hDC, m_tSendButton.l, m_tSendButton.t, m_tSendButton.r, m_tSendButton.b);

	for (int i = 0; i < 5; i++)
	{
		TextOutA(hDC, m_tMessageBox.l, m_tMessageBox.t + 10 + 20 * i, m_strMessages[temp], strlen(m_strMessages[temp]));
		// 인덱스 옮기기
		temp++;
		if (temp >= 5)
			temp = 0;
	}

	pGDIRenderTarget->ReleaseDC(NULL);
}

CUIMain * CUIMain::Clone()
{
	return new CUIMain(*this);
}

void CUIMain::QuickSlot()
{
	if (!m_bQuickChange)
	{
		m_bQuickChange = true;
		m_bQuickExtend = !m_bQuickExtend;
	}	
}

void CUIMain::Setting()
{
	CUI* pUI = FindUI(m_eLastUIType);
	if (pUI)
	{
		// main밖에 안켜져있다면
		if (pUI == this)
		{
			// 세팅창 on/off

		}
		else
			pUI->CloseUI();
	}
}

void CUIMain::Exit()
{
	GET_SINGLE(CCore)->Exit();
}

void CUIMain::AddNums(const string & strTag, const wchar_t * pFileName, POSITION& tOrigin, POSITION& tLT, POSITION& tRB, bool bIsLevel)
{
	int len = wcslen(pFileName);
	wchar_t n[6];
	
	for (int i = 0; i < 10; i++)
	{
		wchar_t* tempFileName = new wchar_t[len + 6];
		wcscpy(tempFileName, pFileName);
		swprintf(n, L"%d.png", i);
		wcscat(tempFileName, n);

		string tempTag = strTag + to_string(i);

		_SIZE tSize = tRB - tLT;
		CTexture* pTex = GET_SINGLE(CResourcesManager)->LoadTexture(tempTag, tempFileName, tOrigin, tSize, tLT, tRB);
		if (pTex)
		{
			// 1인 경우
			if (i == 1)
			{
				// level에 들어가는 1인 경우 크기 차이 보정
				if (bIsLevel)
					pTex->SetSize(_SIZE(tSize.x - 2.f, tSize.y));
				else // 보통 1인 경우 크기 차이 보정
					pTex->SetSize(_SIZE(tSize.x - 3.f, tSize.y));
			}

			if (bIsLevel)
				m_mapLevelNum.insert(make_pair(i, pTex));
			else
				m_mapNum.insert(make_pair(i, pTex));
		}

		delete[] tempFileName;
	}
}

void CUIMain::KeyImageInit()
{
	Safe_Release_Map(m_mapKeyImage);

	POSITION tOrigin = POSITION(0.f, 0.f);
	POSITION tLT = POSITION(0.f, 0.f);
	
	// ESC ~ F12
	m_mapKeyImage.insert(make_pair("ESC", GET_SINGLE(CResourcesManager)->LoadTexture("ESC", L"UI/Key/Key_1.png", tOrigin, _SIZE(24.f, 9.f), tLT, POSITION(24.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F1", GET_SINGLE(CResourcesManager)->LoadTexture("F1", L"UI/Key/Key_59.png", tOrigin, _SIZE(14.f, 9.f), tLT, POSITION(14.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F2", GET_SINGLE(CResourcesManager)->LoadTexture("F2", L"UI/Key/Key_60.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F3", GET_SINGLE(CResourcesManager)->LoadTexture("F3", L"UI/Key/Key_61.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F4", GET_SINGLE(CResourcesManager)->LoadTexture("F4", L"UI/Key/Key_62.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F5", GET_SINGLE(CResourcesManager)->LoadTexture("F5", L"UI/Key/Key_63.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F6", GET_SINGLE(CResourcesManager)->LoadTexture("F6", L"UI/Key/Key_64.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F7", GET_SINGLE(CResourcesManager)->LoadTexture("F7", L"UI/Key/Key_65.png", tOrigin, _SIZE(16.f, 9.f), tLT, POSITION(16.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F8", GET_SINGLE(CResourcesManager)->LoadTexture("F8", L"UI/Key/Key_66.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F9", GET_SINGLE(CResourcesManager)->LoadTexture("F9", L"UI/Key/Key_67.png", tOrigin, _SIZE(17.f, 9.f), tLT, POSITION(17.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F10", GET_SINGLE(CResourcesManager)->LoadTexture("F10", L"UI/Key/Key_68.png", tOrigin, _SIZE(21.f, 9.f), tLT, POSITION(21.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F11", GET_SINGLE(CResourcesManager)->LoadTexture("F11", L"UI/Key/Key_87.png", tOrigin, _SIZE(18.f, 9.f), tLT, POSITION(18.f, 9.f))));
	m_mapKeyImage.insert(make_pair("F12", GET_SINGLE(CResourcesManager)->LoadTexture("F12", L"UI/Key/Key_88.png", tOrigin, _SIZE(21.f, 9.f), tLT, POSITION(21.f, 9.f))));

	// SLK
	m_mapKeyImage.insert(make_pair("SLK", GET_SINGLE(CResourcesManager)->LoadTexture("SLK", L"UI/Key/Key_70.png", tOrigin, _SIZE(19.f, 9.f), tLT, POSITION(19.f, 9.f))));

	// ` ~ Bksp
	m_mapKeyImage.insert(make_pair("`", GET_SINGLE(CResourcesManager)->LoadTexture("`", L"UI/Key/Key_41.png", tOrigin, _SIZE(7.f, 6.f), tLT, POSITION(7.f, 6.f))));
	m_mapKeyImage.insert(make_pair("1", GET_SINGLE(CResourcesManager)->LoadTexture("1", L"UI/Key/Key_2.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("2", GET_SINGLE(CResourcesManager)->LoadTexture("2", L"UI/Key/Key_3.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("3", GET_SINGLE(CResourcesManager)->LoadTexture("3", L"UI/Key/Key_4.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("4", GET_SINGLE(CResourcesManager)->LoadTexture("4", L"UI/Key/Key_5.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("5", GET_SINGLE(CResourcesManager)->LoadTexture("5", L"UI/Key/Key_6.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("6", GET_SINGLE(CResourcesManager)->LoadTexture("6", L"UI/Key/Key_7.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("7", GET_SINGLE(CResourcesManager)->LoadTexture("7", L"UI/Key/Key_8.png", tOrigin, _SIZE(9.f, 9.f), tLT, POSITION(9.f, 9.f))));
	m_mapKeyImage.insert(make_pair("8", GET_SINGLE(CResourcesManager)->LoadTexture("8", L"UI/Key/Key_9.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("9", GET_SINGLE(CResourcesManager)->LoadTexture("9", L"UI/Key/Key_10.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("0", GET_SINGLE(CResourcesManager)->LoadTexture("0", L"UI/Key/Key_11.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("-", GET_SINGLE(CResourcesManager)->LoadTexture("-", L"UI/Key/Key_12.png", tOrigin, _SIZE(7.f, 5.f), tLT, POSITION(7.f, 5.f))));
	m_mapKeyImage.insert(make_pair("=", GET_SINGLE(CResourcesManager)->LoadTexture("=", L"UI/Key/Key_13.png", tOrigin, _SIZE(10.f, 7.f), tLT, POSITION(10.f, 7.f))));
	
	// Tab ~ '\'
	m_mapKeyImage.insert(make_pair("q", GET_SINGLE(CResourcesManager)->LoadTexture("q", L"UI/Key/Key_16.png", tOrigin, _SIZE(10.f, 10.f), tLT, POSITION(10.f, 10.f))));
	m_mapKeyImage.insert(make_pair("w", GET_SINGLE(CResourcesManager)->LoadTexture("w", L"UI/Key/Key_17.png", tOrigin, _SIZE(12.f, 9.f), tLT, POSITION(12.f, 9.f))));
	m_mapKeyImage.insert(make_pair("e", GET_SINGLE(CResourcesManager)->LoadTexture("e", L"UI/Key/Key_18.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("r", GET_SINGLE(CResourcesManager)->LoadTexture("r", L"UI/Key/Key_19.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("t", GET_SINGLE(CResourcesManager)->LoadTexture("t", L"UI/Key/Key_20.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("y", GET_SINGLE(CResourcesManager)->LoadTexture("y", L"UI/Key/Key_21.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("u", GET_SINGLE(CResourcesManager)->LoadTexture("u", L"UI/Key/Key_22.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("i", GET_SINGLE(CResourcesManager)->LoadTexture("i", L"UI/Key/Key_23.png", tOrigin, _SIZE(6.f, 9.f), tLT, POSITION(6.f, 9.f))));
	m_mapKeyImage.insert(make_pair("o", GET_SINGLE(CResourcesManager)->LoadTexture("o", L"UI/Key/Key_24.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("p", GET_SINGLE(CResourcesManager)->LoadTexture("p", L"UI/Key/Key_25.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("[", GET_SINGLE(CResourcesManager)->LoadTexture("[", L"UI/Key/Key_26.png", tOrigin, _SIZE(7.f, 11.f), tLT, POSITION(7.f, 11.f))));
	m_mapKeyImage.insert(make_pair("]", GET_SINGLE(CResourcesManager)->LoadTexture("]", L"UI/Key/Key_27.png", tOrigin, _SIZE(7.f, 11.f), tLT, POSITION(7.f, 11.f))));
	m_mapKeyImage.insert(make_pair("\\", GET_SINGLE(CResourcesManager)->LoadTexture("\\", L"UI/Key/Key_43.png", tOrigin, _SIZE(14.f, 11.f), tLT, POSITION(14.f, 11.f))));
	
	// a ~ Enter
	m_mapKeyImage.insert(make_pair("a", GET_SINGLE(CResourcesManager)->LoadTexture("a", L"UI/Key/Key_30.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("s", GET_SINGLE(CResourcesManager)->LoadTexture("s", L"UI/Key/Key_31.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("d", GET_SINGLE(CResourcesManager)->LoadTexture("d", L"UI/Key/Key_32.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("f", GET_SINGLE(CResourcesManager)->LoadTexture("f", L"UI/Key/Key_33.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("g", GET_SINGLE(CResourcesManager)->LoadTexture("g", L"UI/Key/Key_34.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("h", GET_SINGLE(CResourcesManager)->LoadTexture("h", L"UI/Key/Key_35.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("j", GET_SINGLE(CResourcesManager)->LoadTexture("j", L"UI/Key/Key_36.png", tOrigin, _SIZE(9.f, 9.f), tLT, POSITION(9.f, 9.f))));
	m_mapKeyImage.insert(make_pair("k", GET_SINGLE(CResourcesManager)->LoadTexture("k", L"UI/Key/Key_37.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("l", GET_SINGLE(CResourcesManager)->LoadTexture("l", L"UI/Key/Key_38.png", tOrigin, _SIZE(9.f, 9.f), tLT, POSITION(9.f, 9.f))));
	m_mapKeyImage.insert(make_pair(";", GET_SINGLE(CResourcesManager)->LoadTexture(";", L"UI/Key/Key_39.png", tOrigin, _SIZE(7.f, 10.f), tLT, POSITION(7.f, 10.f))));
	m_mapKeyImage.insert(make_pair("\'", GET_SINGLE(CResourcesManager)->LoadTexture("\'", L"UI/Key/Key_40.png", tOrigin, _SIZE(6.f, 6.f), tLT, POSITION(6.f, 6.f))));

	// Shift ~ .
	m_mapKeyImage.insert(make_pair("Shift", GET_SINGLE(CResourcesManager)->LoadTexture("Shift", L"UI/Key/Key_42.png", tOrigin, _SIZE(28.f, 9.f), tLT, POSITION(28.f, 9.f))));
	m_mapKeyImage.insert(make_pair("z", GET_SINGLE(CResourcesManager)->LoadTexture("z", L"UI/Key/Key_44.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("x", GET_SINGLE(CResourcesManager)->LoadTexture("x", L"UI/Key/Key_45.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("c", GET_SINGLE(CResourcesManager)->LoadTexture("c", L"UI/Key/Key_46.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("v", GET_SINGLE(CResourcesManager)->LoadTexture("v", L"UI/Key/Key_47.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("b", GET_SINGLE(CResourcesManager)->LoadTexture("b", L"UI/Key/Key_48.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("n", GET_SINGLE(CResourcesManager)->LoadTexture("n", L"UI/Key/Key_49.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair("m", GET_SINGLE(CResourcesManager)->LoadTexture("m", L"UI/Key/Key_50.png", tOrigin, _SIZE(10.f, 9.f), tLT, POSITION(10.f, 9.f))));
	m_mapKeyImage.insert(make_pair(",", GET_SINGLE(CResourcesManager)->LoadTexture(",", L"UI/Key/Key_51.png", tOrigin, _SIZE(7.f, 7.f), tLT, POSITION(7.f, 7.f))));
	m_mapKeyImage.insert(make_pair(".", GET_SINGLE(CResourcesManager)->LoadTexture(".", L"UI/Key/Key_52.png", tOrigin, _SIZE(6.f, 5.f), tLT, POSITION(6.f, 5.f))));

	// Ctrl, Alt, Space
	m_mapKeyImage.insert(make_pair("Ctrl", GET_SINGLE(CResourcesManager)->LoadTexture("Ctrl", L"UI/Key/Key_29.png", tOrigin, _SIZE(23.f, 9.f), tLT, POSITION(23.f, 9.f))));
	m_mapKeyImage.insert(make_pair("Alt", GET_SINGLE(CResourcesManager)->LoadTexture("Alt", L"UI/Key/Key_56.png", tOrigin, _SIZE(18.f, 9.f), tLT, POSITION(18.f, 9.f))));
	m_mapKeyImage.insert(make_pair("Space", GET_SINGLE(CResourcesManager)->LoadTexture("Space", L"UI/Key/Key_57.png", tOrigin, _SIZE(34.f, 11.f), tLT, POSITION(34.f, 11.f))));

	// Insert, Home, PgUp
	m_mapKeyImage.insert(make_pair("Ins", GET_SINGLE(CResourcesManager)->LoadTexture("Ins", L"UI/Key/Key_82.png", tOrigin, _SIZE(18.f, 9.f), tLT, POSITION(18.f, 9.f))));
	m_mapKeyImage.insert(make_pair("Hm", GET_SINGLE(CResourcesManager)->LoadTexture("Hm", L"UI/Key/Key_71.png", tOrigin, _SIZE(19.f, 9.f), tLT, POSITION(19.f, 9.f))));
	m_mapKeyImage.insert(make_pair("Pup", GET_SINGLE(CResourcesManager)->LoadTexture("Pup", L"UI/Key/Key_73.png", tOrigin, _SIZE(22.f, 11.f), tLT, POSITION(22.f, 11.f))));

	// Del, End, PgDn
	m_mapKeyImage.insert(make_pair("Del", GET_SINGLE(CResourcesManager)->LoadTexture("Del", L"UI/Key/Key_83.png", tOrigin, _SIZE(19.f, 9.f), tLT, POSITION(19.f, 9.f))));
	m_mapKeyImage.insert(make_pair("End", GET_SINGLE(CResourcesManager)->LoadTexture("End", L"UI/Key/Key_79.png", tOrigin, _SIZE(22.f, 9.f), tLT, POSITION(22.f, 9.f))));
	m_mapKeyImage.insert(make_pair("Pdn", GET_SINGLE(CResourcesManager)->LoadTexture("Pdn", L"UI/Key/Key_81.png", tOrigin, _SIZE(22.f, 9.f), tLT, POSITION(22.f, 9.f))));
}

void CUIMain::QuickSlotInit()
{
	// POSTIION
	// (0,0) ~ (15,0)
	// (0,1) ~ (15,1)
	// 처음 위치 : (0.f, 0.f), 간격 : 상하 35.f, 좌우 35.f
	m_mapQuickPos.insert(make_pair(POSITION(0.f, 0.f), "Shift"));
	m_mapQuickPos.insert(make_pair(POSITION(1.f, 0.f), "Ins"));
	m_mapQuickPos.insert(make_pair(POSITION(2.f, 0.f), "Hm"));
	m_mapQuickPos.insert(make_pair(POSITION(3.f, 0.f), "Pup"));
	m_mapQuickPos.insert(make_pair(POSITION(4.f, 0.f), "1"));
	m_mapQuickPos.insert(make_pair(POSITION(5.f, 0.f), "2"));
	m_mapQuickPos.insert(make_pair(POSITION(6.f, 0.f), "3"));
	m_mapQuickPos.insert(make_pair(POSITION(7.f, 0.f), "4"));
	m_mapQuickPos.insert(make_pair(POSITION(8.f, 0.f), "5"));
	m_mapQuickPos.insert(make_pair(POSITION(9.f, 0.f), "6"));
	m_mapQuickPos.insert(make_pair(POSITION(10.f, 0.f), "7"));
	m_mapQuickPos.insert(make_pair(POSITION(11.f, 0.f), "8"));
	m_mapQuickPos.insert(make_pair(POSITION(12.f, 0.f), "9"));
	m_mapQuickPos.insert(make_pair(POSITION(13.f, 0.f), "0"));
	m_mapQuickPos.insert(make_pair(POSITION(14.f, 0.f), "-"));
	m_mapQuickPos.insert(make_pair(POSITION(15.f, 0.f), "="));
	
	m_mapQuickPos.insert(make_pair(POSITION(0.f, 1.f), "Ctrl"));
	m_mapQuickPos.insert(make_pair(POSITION(1.f, 1.f), "Del"));
	m_mapQuickPos.insert(make_pair(POSITION(2.f, 1.f), "End"));
	m_mapQuickPos.insert(make_pair(POSITION(3.f, 1.f), "Pdn"));
	m_mapQuickPos.insert(make_pair(POSITION(4.f, 1.f), "a"));
	m_mapQuickPos.insert(make_pair(POSITION(5.f, 1.f), "s"));
	m_mapQuickPos.insert(make_pair(POSITION(6.f, 1.f), "d"));
	m_mapQuickPos.insert(make_pair(POSITION(7.f, 1.f), "f"));
	m_mapQuickPos.insert(make_pair(POSITION(8.f, 1.f), "x"));
	m_mapQuickPos.insert(make_pair(POSITION(9.f, 1.f), "c"));
	m_mapQuickPos.insert(make_pair(POSITION(10.f, 1.f), "v"));
	m_mapQuickPos.insert(make_pair(POSITION(11.f, 1.f), "b"));
	m_mapQuickPos.insert(make_pair(POSITION(12.f, 1.f), "n"));
	m_mapQuickPos.insert(make_pair(POSITION(13.f, 1.f), "h"));
	m_mapQuickPos.insert(make_pair(POSITION(14.f, 1.f), "t"));
	m_mapQuickPos.insert(make_pair(POSITION(15.f, 1.f), "j"));
}

string CUIMain::FindQuickTag(POSITION tPos)
{
	map<POSITION, string>::iterator iter;
	map<POSITION, string>::iterator iterEnd = m_mapQuickPos.end();

	for (iter = m_mapQuickPos.begin(); iter != iterEnd; iter++)
	{
		POSITION tQuickPos = FindPosition("QuickBack");
		RECTANGLE tRect;
		tRect.l = tQuickPos.x + iter->first.x * 35.f;
		tRect.t = tQuickPos.y + iter->first.y * 35.f;
		tRect.r = tRect.l + 32.f;
		tRect.b = tRect.t + 32.f;
		if (PointInRect(tRect, tPos))
			return iter->second;			
	}

	return "";
}
