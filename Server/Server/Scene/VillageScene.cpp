#include "VillageScene.h"

#include "../Object/StaticObj/Stage/Village/Village.h"
#include "../Object/StaticObj/ShopNPC/ShopNPC.h"

CVillageScene::CVillageScene()
{
}


CVillageScene::~CVillageScene()
{
}

bool CVillageScene::Init()
{
	if (!CScene::Init())
		return false;

	SetNum(0);

	SetSceneRS(RESOLUTION(1365, 766));
	SetDefaultPos(POSITION(400.f, 606.f));
	SetYPosMax(607.f);

	CStage* pMainMap = CObj::CreateObj<CVillage>("Village", this);
	SAFE_RELEASE(pMainMap);

	return true;
}
