#include "MushmomStage.h"
#include "../../../../Collider/ColliderPixel.h"

CMushmomStage::CMushmomStage()
{
}

CMushmomStage::CMushmomStage(const CMushmomStage & stage) :
	CStage(stage)
{
}

CMushmomStage::~CMushmomStage()
{
}

bool CMushmomStage::Init()
{
	CStage::Init();

	// 1890 x 1080
	SetSize(1890.f, 1080.f);
	SetPos(0.f, 0.f);
	SetPivot(0.f, 0.f);
	Sethw(0.f, 0.f);
	SetObjType(OT_MAP);

	SetTexture("MushmomStage", L"Map/MushmomStage/MushmomStage.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(1890.f, 1080.f));
	SetMinimapTexture("MushmomMinimap", L"Map/MushmomStage/Minimap.png", POSITION(0.f, 0.f), POSITION(0.f, 0.f), POSITION(124.f, 72.f));

	//
	//								floor1
	//	------floor2-------						floor3
	//													----floor4----
	//						--floor5--		floor6
	//
	//					------------floor7------------
	//
	//	----floor8----
	//
	//
	//------------------------------main----------------------------

	//PIXEL pMagenta = PIXEL(255, 0, 255);

	//CColliderPixel* pPixel = AddCollider<CColliderPixel>("MainFloor");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/MainFloor.bmp");
	//pPixel->MakePixelVec(1890, 1, pMagenta);
	//pPixel->SetPos(0.f, 876.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor1");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor1.bmp");
	//pPixel->MakePixelVec(322, 1, pMagenta);
	//pPixel->SetPos(828.f, 246.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor2");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor2.bmp");
	//pPixel->MakePixelVec(682, 1, pMagenta);
	//pPixel->SetPos(110.f, 306.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor3");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor3.bmp");
	//pPixel->MakePixelVec(144, 1, pMagenta);
	//pPixel->SetPos(1188.f, 306.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor4");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor4.bmp");
	//pPixel->MakePixelVec(504, 1, pMagenta);
	//pPixel->SetPos(1368.f, 366.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor5");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor5.bmp");
	//pPixel->MakePixelVec(504, 1, pMagenta);
	//pPixel->SetPos(739.f, 426.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor6");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor6.bmp");
	//pPixel->MakePixelVec(55, 1, pMagenta);
	//pPixel->SetPos(1278.f, 426.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor7");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor7.bmp");
	//pPixel->MakePixelVec(680, 1, pMagenta);
	//pPixel->SetPos(649.f, 606.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	//pPixel = AddCollider<CColliderPixel>("Floor8");
	////pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor/Floor8.bmp");
	//pPixel->MakePixelVec(506, 1, pMagenta);
	//pPixel->SetPos(107.f, 666.f);
	//pPixel->SetHitColor(pMagenta);
	//SAFE_RELEASE(pPixel);

	CColliderPixel* pPixel = AddCollider<CColliderPixel>("Floor");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Floor.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	/*pPixel = AddCollider<CColliderPixel>("MonsterWall");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/MonsterWall.bmp");
	pPixel->SetHitColor(PIXEL(0, 0, 0));
	SAFE_RELEASE(pPixel);*/

	pPixel = AddCollider<CColliderPixel>("Rope");
	pPixel->SetPixelInfo("../Bin/Texture/Map/MushmomStage/Rope.bmp");
#ifndef _DEBUG	
	pPixel->SetHitColor(PIXEL(0, 255, 255));
#else
	pPixel->SetHitColor(PIXEL(255, 255, 0));
#endif
	SAFE_RELEASE(pPixel);

	/*m_fMonsterGenLimitTime = 4.f;
	m_fBossMonsterGenLimitTime = 10.f;*/

	return true;
}

void CMushmomStage::Input(float fDeltaTime)
{
	CStage::Input(fDeltaTime);
}

int CMushmomStage::Update(float fDeltaTime)
{
	CStage::Update(fDeltaTime);

	/*m_fMonsterGenTime += fDeltaTime;
	if (m_fMonsterGenTime > m_fMonsterGenLimitTime)
	{
		m_fMonsterGenTime = 0.f;
		for (int i = 0; i < m_iGen; i++)
		{
			CMonster* pMonster = m_vecMonster[i];
			if (!pMonster->GetEnable())
			{
				pMonster->SetPos(pMonster->GetOriginPos());
				pMonster->SetHP(pMonster->GetMaxHP());
				pMonster->SetMP(pMonster->GetMaxMP());
				pMonster->SetPhysics(false);
				pMonster->SetEnable(true);
			}
		}
	}

	m_fBossMonsterGenTime += fDeltaTime;
	if (m_fBossMonsterGenTime > m_fBossMonsterGenLimitTime)
	{
		m_fBossMonsterGenTime = 0.f;

		if (!m_pBossMonster->GetEnable())
		{
			m_pBossMonster->SetPos(m_pBossMonster->GetOriginPos());
			m_pBossMonster->SetHP(m_pBossMonster->GetMaxHP());
			m_pBossMonster->SetMP(m_pBossMonster->GetMaxMP());
			m_pBossMonster->SetPhysics(false);
			m_pBossMonster->SetEnable(true);
		}
	}*/

	return 0;
}

int CMushmomStage::LateUpdate(float fDeltaTime)
{
	CStage::LateUpdate(fDeltaTime);
	return 0;
}

void CMushmomStage::Collision(float fDeltaTime)
{
	CStage::Collision(fDeltaTime);
}

void CMushmomStage::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStage::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CStage * CMushmomStage::Clone()
{
	return new CMushmomStage(*this);
}