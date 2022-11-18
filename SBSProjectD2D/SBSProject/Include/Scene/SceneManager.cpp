#include "SceneManager.h"

#include "TitleScene.h"
#include "InGameScene.h"
#include "MushroomScene.h"
#include "MushmomScene.h"

#include "../Object/Obj.h"
#include "../Object/MoveObj/Player/Player.h"

#include "Scene.h"
#include "Layer.h"

#include "../Core/Camera.h"
#include "../Core/BackCam.h"

#include "../Core/Core.h"

#include "../Object/StaticObj/UI/UI.h"
#include "../Object/StaticObj/UI/UIMain.h"
#include "../Object/StaticObj/UI/UIStat.h"
#include "../Object/StaticObj/UI/UISkill.h"
#include "../Object/StaticObj/UI/UIKey.h"
#include "../Object/StaticObj/UI/UIItem.h"
#include "../Object/StaticObj/UI/UIMinimap.h"
#include "../Object/StaticObj/UI/UIEquip.h"

#include "../Resources/Sound.h"

DEF_SINGLE(CSceneManager);

CSceneManager::CSceneManager() :
	m_pPlayer(NULL),
	m_pCurrentScene(NULL),
	m_pCurrentBGM(NULL),
	m_fSceneChangeTime(0.f),
	m_fSceneChangeLimitTime(1.f),
	m_iSceneToChange(-1),
	m_bActivateChanged(false)
{
}

CSceneManager::~CSceneManager()
{
	Safe_Delete_Map(m_mapScene);
	SAFE_RELEASE(m_pPlayer);
	CObj::EraseObj(m_pPlayer);
}

void CSceneManager::SetPlayerName(const char * strName)
{
	if (m_pPlayer)
	{
		string strTag(strName);
		m_pPlayer->SetTag(strTag);
	}
}

string CSceneManager::GetPlayerName() const
{
	return m_pPlayer->GetTag();
}

void CSceneManager::SetPlayerChatMode(bool bChatMode)
{
	m_pPlayer->m_bChatMode = bChatMode;
}

bool CSceneManager::IsPlayer(CObj * pObj)
{
	return (m_pPlayer == pObj);
}

void CSceneManager::StopBGM()
{
	if (m_pCurrentBGM)
		m_pCurrentBGM->Stop();
}

bool CSceneManager::Init()
{
	CPlayer* pPlayer = (CPlayer*)CObj::CreateObj<CPlayer>("Player");
	if (!pPlayer)
		return false;
	GET_SINGLE(CCamera)->SetTarget(pPlayer);
	GET_SINGLE(CBackCam)->SetTarget(pPlayer);

	m_pPlayer = pPlayer;

	CreateUIs();

	m_pCurrentScene = CreateScene<CTitleScene>("Title");
	CreateScene<CInGameScene>("Village");
	CreateScene<CMushroomScene>("MushroomStage");
	CreateScene<CMushmomScene>("MushmomStage");

	CScene* pScene = FindScene(INT_MAX);
	if (pScene)
	{
		pScene->SetPlayer(m_pPlayer);
		m_pPlayer->SetEnable(false);

		m_pCurrentBGM = pScene->GetBGM();
		if (m_pCurrentBGM)
			m_pCurrentBGM->Play();
	}

	return true;
}

void CSceneManager::Input(float fDeltaTime)
{
	m_pCurrentScene->Input(fDeltaTime);
}

int CSceneManager::Update(float fDeltaTime)
{
	if (m_iSceneToChange != -1)
	{
		if (m_pCurrentScene->GetTag() == "Title")
			m_pPlayer->SetEnable(true);
		ChangeScene();
	}

	m_pCurrentScene->Update(fDeltaTime);


	if (m_pCurrentBGM)
	{
		m_pCurrentBGM->Update();
		if (m_bActivateChanged)
		{
			if (GET_SINGLE(CCore)->GetActivate())
				m_pCurrentBGM->SetVolume(0); // MAX
			else
				m_pCurrentBGM->SetVolume(-10000); // MIN(mute)
			m_bActivateChanged = false;
		}
	}

	return 0;
}

int CSceneManager::LateUpdate(float fDeltaTime)
{
	m_pCurrentScene->LateUpdate(fDeltaTime);

	return 0;
}

void CSceneManager::Collision(float fDeltaTime)
{
	m_pCurrentScene->Collision(fDeltaTime);
}

void CSceneManager::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	m_pCurrentScene->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CScene * CSceneManager::FindScene(int iNum)
{
	map<int, CScene*>::iterator iter = m_mapScene.find(iNum);

	if (iter == m_mapScene.end())
		return NULL;

	return iter->second;
}
void CSceneManager::ChangeScene()
{
	// ���� scene�� SceneNum ����
	int iSceneNum = m_pCurrentScene->GetNum();

	// ���� Scene �뷡 ����
	if (m_pCurrentBGM)
		m_pCurrentBGM->Stop();

	// ���� scene�� �ִ� Otherplayer ��ü �����ֱ�
	m_pCurrentScene->KillOtherPlayer();

	// ���� Scene���� player ����
	m_pCurrentScene->ReleasePlayer();

	// �ٲ� scene ã��
	CScene* pScene = FindScene(m_iSceneToChange);
	if (!pScene)
		return;

	// �ٲ� scene�� ���� scene���� ����
	m_pCurrentScene = pScene;
	
	// �ٲ� scene�� �뷡�� ���� scene bgm���� ����
	m_pCurrentBGM = pScene->GetBGM();
	if (m_pCurrentBGM)
		m_pCurrentBGM->Play();

	// player�� ���� scene�� object layer�� �߰�
	m_pCurrentScene->SetPlayer(m_pPlayer, iSceneNum);

	// ���� scene�� �°� ī�޶� �ػ� ����
	GET_SINGLE(CCamera)->SetWorldResolution(m_pCurrentScene->GetSceneRS());
	GET_SINGLE(CBackCam)->SetWorldResolution(m_pCurrentScene->GetSceneBackRS());

	// ���� �Ϸ� �� ���� scene string �ʱ�ȭ
	m_iSceneToChange = -1;
}

void CSceneManager::CreateUIs()
{
	CUIMain* pUIMain = CObj::CreateObj<CUIMain>("UIMain");
	SAFE_RELEASE(pUIMain);
	CUIStat* pUIStat = CObj::CreateObj<CUIStat>("UIStat");
	SAFE_RELEASE(pUIStat);
	CUISkill* pUISkill = CObj::CreateObj<CUISkill>("UISkill");
	SAFE_RELEASE(pUISkill);
	CUIKey* pUIKey = CObj::CreateObj<CUIKey>("UIKey");
	SAFE_RELEASE(pUIKey);
	CUIItem* pUIItem = CObj::CreateObj<CUIItem>("UIItem");
	SAFE_RELEASE(pUIItem);
	CUIMinimap* pUIMinimap = CObj::CreateObj<CUIMinimap>("UIMinimap");
	SAFE_RELEASE(pUIMinimap);
	CUIEquip* pUIEquip = CObj::CreateObj<CUIEquip>("UIEquip");
	SAFE_RELEASE(pUIEquip);
}