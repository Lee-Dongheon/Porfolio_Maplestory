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

	CColliderPixel* pPixel = AddCollider<CColliderPixel>("Floor");
	pPixel->SetPixelInfo("../MapCollider/MushmomStage/Floor.bmp");
	pPixel->SetHitColor(PIXEL(255, 0, 255));
	SAFE_RELEASE(pPixel);

	pPixel = AddCollider<CColliderPixel>("MonsterWall");
	pPixel->SetPixelInfo("../MapCollider/MushmomStage/MonsterWall.bmp");
	pPixel->SetHitColor(PIXEL(0, 0, 0));
	SAFE_RELEASE(pPixel);

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

//void CMushmomStage::Collision(float fDeltaTime)
//{
//	CStage::Collision(fDeltaTime);
//}

CStage * CMushmomStage::Clone()
{
	return new CMushmomStage(*this);
}