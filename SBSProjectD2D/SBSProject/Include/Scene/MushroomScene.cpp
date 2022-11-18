#include "MushroomScene.h"
#include "../Object/MoveObj/Player/Player.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "../Core/BackCam.h"
#include "../Object/StaticObj/Stage/MushroomStage/MushroomStage.h"
#include "../Object/StaticObj/Background/Background.h"
#include "../Object/MoveObj/Monster/Mushroom/Mushroom.h"
#include "../Object/StaticObj/Portal/Portal.h"
#include "../Scene/SceneManager.h"
#include "../Resources/Sound.h"
#include "../Resources/ResourcesManager.h"

CMushroomScene::CMushroomScene()
{
}

CMushroomScene::~CMushroomScene()
{
	//m_pBGM->Stop();
}

bool CMushroomScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(1);

	SetSceneRS(RESOLUTION(2250, 1375));
	SetSceneBackRS(RESOLUTION(2250, 748));

	SetDefaultPos(POSITION(1004.f, 1116.f));
	SetYPosMax(1117.f);

	CLayer* pBackgroundLayer = FindLayer("Background");
	CBackground* pBack = CObj::CreateObj<CBackground>("MushroomBackground", pBackgroundLayer);
	pBack->SetTexture("MushroomBackground", L"Map/MushroomStage/MushroomBackground.png",
		POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(2250.f, 748.f));
	pBack->SetSize(2250.f, 748.f);
	SAFE_RELEASE(pBack);

	CLayer* pLayer = FindLayer("Object");
	CStage* pMainMap = CObj::CreateObj<CMushroomStage>("MushroomStage", pLayer);

	CPortal* pPortal = CObj::CreateObj<CPortal>("Portal", pLayer);
	pPortal->SetPos(2150.f, 1115.f);
	pPortal->SetSceneNum(0);
	SAFE_RELEASE(pPortal);

	pPortal = CObj::CreateObj<CPortal>("Portal2", pLayer);
	pPortal->SetPos(180.f, 367.f);
	pPortal->SetSceneNum(2);
	SAFE_RELEASE(pPortal);

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(414.f, 666.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(552.f, 666.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(951.f, 246.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1321.f, 246.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1413.f, 246.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1047.f, 546.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1240.f, 546.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1840.f, 606.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(2008.f, 606.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(2106.f, 606.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(198.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(411.f, 1116.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(957.f, 966.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1167.f, 966.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1417.f, 966.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(849.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1079.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1276.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1466.f, 1116.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", pLayer, POSITION(1776.f, 1116.f));

	m_pBGM = GET_SINGLE(CResourcesManager)->LoadSoundTrack("RestNPeace", L"BGM/Henesis/RestNPeace.mp3");

	SAFE_RELEASE(pMainMap);
	
	return true;
}
