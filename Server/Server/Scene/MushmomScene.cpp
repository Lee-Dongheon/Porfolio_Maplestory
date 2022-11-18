#include "MushmomScene.h"

#include "../Object/StaticObj/Stage/MushmomStage/MushmomStage.h"

#include "../Object/MoveObj/Monster/GreenMushroom/GreenMushroom.h"
#include "../Object/MoveObj/Monster/Mushmom/Mushmom.h"


CMushmomScene::CMushmomScene()
{
}


CMushmomScene::~CMushmomScene()
{
}

bool CMushmomScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(2);

	SetSceneRS(RESOLUTION(1890, 1080));
	SetDefaultPos(POSITION(1055.f, 246.f));
	SetYPosMax(880.f);

	CStage* pMainMap = CObj::CreateObj<CMushmomStage>("MushmomStage", this);

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(191.f, 306.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(380.f, 306.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(610.f, 306.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(913.f, 426.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1049.f, 426.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1438.f, 366.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1566.f, 366.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1754.f, 366.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(207.f, 666.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(437.f, 666.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(707.f, 606.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1003.f, 606.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1244.f, 606.f));

	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(141.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(472.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(736.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(987.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1322.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1560.f, 876.f));
	AddMonster<CGreenMushroom, CStage>(pMainMap, "GreenMushroom", this, POSITION(1763.f, 876.f));

	AddBossMonster<CMushmom, CStage>(pMainMap, "Mushmom", this, POSITION(1212.f, 875.f));
	SAFE_RELEASE(pMainMap);

	return true;
}
