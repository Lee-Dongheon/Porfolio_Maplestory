#include "TitleScene.h"
#include "../Object/Obj.h"
#include "../Object/StaticObj/UI/TitleUI.h"
#include "../Resources/ResourcesManager.h"

#include "Layer.h"

CTitleScene::CTitleScene()
{
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::Init()
{
	SetNum(INT_MAX);

	CLayer* pLayer = FindLayer("Object");
	CTitleUI* pUI = CObj::CreateObj<CTitleUI>("TitleUIObj", pLayer);

	SetDefaultPos(POSITION(400.f, 605.f));
	SetYPosMax(606.f);

	SAFE_RELEASE(pUI);

	m_pBGM = GET_SINGLE(CResourcesManager)->LoadSoundTrack("LoginTheme", L"BGM/Login/LoginTheme.mp3");

	return true;
}
