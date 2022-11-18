#include "MushmomScene.h"
#include "../Object/MoveObj/Player/Player.h"
#include "Layer.h"
#include "../Core/Camera.h"
#include "../Core/BackCam.h"
#include "../Object/StaticObj/Stage/MushmomStage/MushmomStage.h"
#include "../Object/StaticObj/Background/Background.h"
#include "../Object/MoveObj/Monster/Mushmom/Mushmom.h"
#include "../Object/MoveObj/Monster/GreenMushroom/GreenMushroom.h"
#include "../Object/StaticObj/Portal/Portal.h"
#include "../Scene/SceneManager.h"
#include "../Resources/Sound.h"
#include "../Resources/ResourcesManager.h"

CMushmomScene::CMushmomScene()
{
}

CMushmomScene::~CMushmomScene()
{
	//m_pBGM->Stop();
}

bool CMushmomScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(2);

	SetSceneRS(RESOLUTION(1890, 1080));
	SetSceneBackRS(RESOLUTION(1890, 748));

	SetDefaultPos(POSITION(1055.f, 246.f));
	SetYPosMax(880.f);

	CLayer* pBackgroundLayer = FindLayer("Background");
	CBackground* pBack = CObj::CreateObj<CBackground>("MushmomBackground", pBackgroundLayer);
	pBack->SetTexture("MushmomBackground", L"Map/MushmomStage/MushmomBackground.png",
		POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(1890.f, 748.f));
	pBack->SetSize(1890.f, 748.f);
	SAFE_RELEASE(pBack);

	CLayer* pLayer = FindLayer("Object");
	CStage* pMainMap = CObj::CreateObj<CMushmomStage>("MushmomStage", pLayer);

	CPortal* pPortal = CObj::CreateObj<CPortal>("Portal", pLayer);
	pPortal->SetPos(1055.f, 245.f);
	pPortal->SetSceneNum(1);
	SAFE_RELEASE(pPortal);

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(191.f, 306.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(380.f, 306.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(610.f, 306.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(913.f, 426.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1049.f, 426.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1438.f, 366.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1566.f, 366.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1754.f, 366.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(207.f, 666.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(437.f, 666.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(707.f, 606.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1003.f, 606.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1244.f, 606.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(141.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(472.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(736.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(987.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1322.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1560.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", pLayer, POSITION(1763.f, 876.f));
	
	AddBossMonster<CMushmom, CStage>(pMainMap, "Mushmom", pLayer, POSITION(1212.f, 875.f));
	SAFE_RELEASE(pMainMap);

	m_pBGM = GET_SINGLE(CResourcesManager)->LoadSoundTrack("FloralLife", L"BGM/Henesis/FloralLife.mp3");

	return true;
}
