#include "InGameScene.h"
#include "../Object/MoveObj/Player/Player.h"
#include "Layer.h"
#include "../Object/StaticObj/Stage/Village/Village.h"
#include "../Object/StaticObj/Background/Background.h"
#include "../Object/StaticObj/Portal/Portal.h"
#include "../Scene/SceneManager.h"
#include "../Resources/Sound.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/StaticObj/MayaNPC/MayaNPC.h"


CInGameScene::CInGameScene()
{
}

CInGameScene::~CInGameScene()
{
}

bool CInGameScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(0);

	SetSceneRS(RESOLUTION(1365, 766));
	SetSceneBackRS(RESOLUTION(1365, 766));

	SetDefaultPos(POSITION(400.f, 606.f));
	SetYPosMax(607.f);

	CLayer* pBackgroundLayer = FindLayer("Background");
	CBackground* pBack = CObj::CreateObj<CBackground>("VillageBackground", pBackgroundLayer);
	pBack->SetTexture("VillageBackground", L"Map/Village/VillageBackground.png",
		POSITION(0.f, 0.f), POSITION(0.f,0.f), POSITION(1365.f, 766.f));
	pBack->SetSize(1365.f, 766.f);
	SAFE_RELEASE(pBack);

	CLayer* pLayer = FindLayer("Object");
	CStage* pMainMap = CObj::CreateObj<CVillage>("Village", pLayer);
	SAFE_RELEASE(pMainMap);
	
	CMayaNPC* pNPC = CObj::CreateObj<CMayaNPC>("NPC", pLayer);
	pNPC->SetPos(685.f, 406.f);
	SAFE_RELEASE(pNPC);

	CPortal* pPortal = CObj::CreateObj<CPortal>("Portal", pLayer);
	pPortal->SetPos(917.f, 605.f);
	pPortal->SetSceneNum(1);
	SAFE_RELEASE(pPortal);

	m_pBGM = GET_SINGLE(CResourcesManager)->LoadSoundTrack("FloralLife", L"BGM/Henesis/FloralLife.mp3");

	return true;
}