#include "Village.h"
#include "../../../../Collider/ColliderPixel.h"

CVillage::CVillage()
{
}

CVillage::CVillage(const CVillage & village):
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
	SetObjType(OT_MAP);

	SetTexture("Village", L"Map/Village/Village.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(1365.f, 766.f));
	SetMinimapTexture("VillageMinimap", L"Map/Village/Minimap.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(91.f, 60.f));

	CColliderPixel* pPixel = AddCollider<CColliderPixel>("Floor");
	pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("Wall");
	pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Wall.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

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

	//PIXEL pMagenta = PIXEL(255, 0, 255);

	//CColliderPixel* pPixel = AddCollider<CColliderPixel>("MainFloor");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/MainFloor.bmp");
	//pPixel->MakePixelVec(1365, 1, pMagenta);
	//pPixel->SetPos(0.f, 606.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("MayaFloor");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/MayaFloor.bmp");
	//pPixel->MakePixelVec(593, 1, pMagenta);
	//pPixel->SetPos(387.f, 405.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor1");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(708.f, 586.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor2");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(724.f, 563.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor3");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(740.f, 542.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	////midstep
	//pPixel = AddCollider<CColliderPixel>("Floor4");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/ex/MidFloor.bmp");
	//pPixel->MakePixelVec(154, 1, pMagenta);
	//pPixel->SetPos(756.f, 520.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor5");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(814.f, 498.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor6");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(798.f, 477.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor7");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(782.f, 454.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor8");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/Village/Floor/Step.bmp");
	//pPixel->MakePixelVec(77, 1, pMagenta);
	//pPixel->SetPos(765.f, 433.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("FloorWall1");
	//pPixel->MakePixelVec(1, 405, pMagenta);
	//pPixel->SetPos(387.f, 0.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("FloorWall2");
	//pPixel->MakePixelVec(1, 405, pMagenta);
	//pPixel->SetPos(979.f, 0.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	// m_fMonsterGenLimitTime = 4.f;

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

void CVillage::Collision(float fDeltaTime)
{
	CStage::Collision(fDeltaTime);
}

void CVillage::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStage::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CStage * CVillage::Clone()
{
	return new CVillage(*this);
}
