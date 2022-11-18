#include "Village.h"
#include "../../../../Collider/ColliderPixel.h"

CVillage::CVillage()
{
}

CVillage::CVillage(const CVillage & village) :
	CStage(village)
{
}

CVillage::~CVillage()
{
}

bool CVillage::Init()
{
	CStage::Init();

	// 1365 x 766
	SetSize(1365.f, 766.f);
	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);
	Sethw(0.f, 0.f);

	/*CColliderPixel* pPixel = AddCollider<CColliderPixel>("Floor");
	pPixel->SetPixelInfo("../MapCollider/Village/Floor.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("Wall");
	pPixel->SetPixelInfo("../MapCollider/Village/Wall.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);*/

	//			wall										wall
	//			--------------------maya------------------------
	//										step8
	//											step7
	//												step6
	//													step5
	//												midstep4
	//											step3
	//										step2
	//									step1
	//------------------------------main--------------------------------
	//

	return true;
}

void CVillage::Input(float fDeltaTime)
{
	CStage::Input(fDeltaTime);
}

int CVillage::Update(float fDeltaTime)
{
	CStage::Update(fDeltaTime);

	return 0;
}

int CVillage::LateUpdate(float fDeltaTime)
{
	CStage::LateUpdate(fDeltaTime);
	return 0;
}

//void CVillage::Collision(float fDeltaTime)
//{
//	CStage::Collision(fDeltaTime);
//}

CStage * CVillage::Clone()
{
	return new CVillage(*this);
}
