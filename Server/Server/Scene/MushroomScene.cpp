#include "MushroomScene.h"
#include "../Object/StaticObj/Stage/MushroomStage/MushroomStage.h"
#include "../Object/MoveObj/Monster/Mushroom/Mushroom.h"

CMushroomScene::CMushroomScene()
{
}


CMushroomScene::~CMushroomScene()
{
}

bool CMushroomScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(1);
	SetSceneRS(RESOLUTION(2250, 1375));
	SetDefaultPos(POSITION(1004.f, 1116.f));
	SetYPosMax(1117.f);

	CStage* pMainMap = CObj::CreateObj<CMushroomStage>("MushroomStage", this);

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(414.f, 666.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(552.f, 666.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(951.f, 246.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1321.f, 246.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1413.f, 246.f));
	
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1047.f, 546.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1240.f, 546.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1840.f, 606.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(2008.f, 606.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(2106.f, 606.f));
	
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(198.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(411.f, 1116.f));
	
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(957.f, 966.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1167.f, 966.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1417.f, 966.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(849.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1079.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1276.f, 1116.f));
	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1466.f, 1116.f));

	AddMonster<CMushroom, CStage>(pMainMap, "Mushroom", this, POSITION(1776.f, 1116.f));

	SAFE_RELEASE(pMainMap);

	return true;
}
