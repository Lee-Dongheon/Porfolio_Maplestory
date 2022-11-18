#include "MushroomStage.h"
#include "../../../../Collider/ColliderPixel.h"

CMushroomStage::CMushroomStage()
{
}

CMushroomStage::CMushroomStage(const CMushroomStage & stage) :
	CStage(stage)
{
}

CMushroomStage::~CMushroomStage()
{
}

bool CMushroomStage::Init()
{
	CStage::Init();

	// 2250 x 1375
	SetSize(2250.f, 1375.f);

	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);
	Sethw(0.f, 0.f);
	SetObjType(OT_MAP);

	SetTexture("MushroomStage", L"Map/MushroomStage/MushroomStage.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(2250.f, 1375.f));
	SetMinimapTexture("MushroomMinimap", L"Map/MushroomStage/Minimap.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(140.f, 82.f));

	//
	//						---------floor1---------
	//
	//----floor2----										----floor3----
	//				floor4
	//					floor5						floor6
	//						---------floor7---------	floor8
	//					floor9								------floor10------
	//		---floor11---
	//
	//
	//			floor12										floor13
	//					floor14						floor15
	//							  centerfloor
	//
	//--------------------------------main------------------------------
	//

	CColliderPixel* pPixel = AddCollider<CColliderPixel>("Floor");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushroomStage/Floor.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("Wall");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushroomStage/Wall.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("Ladder");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushroomStage/Ladder.bmp");
#ifndef _DEBUG
	pPixel->SetHitColor(PIXEL(0, 255, 255));
#else
	pPixel->SetHitColor(PIXEL(255, 255, 0));
#endif
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("Rope");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushroomStage/Rope.bmp");
#ifndef _DEBUG	
	pPixel->SetHitColor(PIXEL(0, 255, 255));
#else
	pPixel->SetHitColor(PIXEL(255, 255, 0));
#endif
	SAFE_RELEASE(pPixel);

	m_fMonsterGenTime = 4.f;

	return true;
}

void CMushroomStage::Input(float fDeltaTime)
{
	CStage::Input(fDeltaTime);
}

int CMushroomStage::Update(float fDeltaTime)
{
	CStage::Update(fDeltaTime);

	return 0;
}

int CMushroomStage::LateUpdate(float fDeltaTime)
{
	CStage::LateUpdate(fDeltaTime);
	return 0;
}

void CMushroomStage::Collision(float fDeltaTime)
{
	CStage::Collision(fDeltaTime);
}

void CMushroomStage::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStage::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CStage * CMushroomStage::Clone()
{
	return new CMushroomStage(*this);
}