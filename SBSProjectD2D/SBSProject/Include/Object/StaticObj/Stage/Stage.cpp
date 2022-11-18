#include "Stage.h"
#include "../../../Resources/Texture.h"
#include "../../../Core/Core.h"
#include "../../../Core/Camera.h"
#include "../../../Collider/ColliderRect.h"
#include "../../../Scene/SceneManager.h"
#include "../../MoveObj/Monster/Monster.h"
#include "../../../Resources/ResourcesManager.h"

CStage::CStage():
	m_fMonsterGenTime(0.f),
	m_fMonsterGenLimitTime(7.f),
	m_fBossMonsterGenTime(0.f),
	m_fBossMonsterGenLimitTime(60.f),
	m_iGen(0),
	m_pBossMonster(NULL),
	m_pMinimapTexture(NULL)
{
}

CStage::CStage(const CStage & stage) :
	CStaticObj(stage)
{
	m_fMonsterGenTime = stage.m_fMonsterGenTime;
	m_fMonsterGenLimitTime = stage.m_fMonsterGenLimitTime;
	m_iGen = stage.m_iGen;
	m_pMinimapTexture = NULL;
	if (stage.m_pMinimapTexture)
	{
		m_pMinimapTexture = stage.m_pMinimapTexture;
		m_pMinimapTexture->AddRef();
	}
}

CStage::~CStage()
{
	Safe_Release_VecList(m_vecMonster);
	SAFE_RELEASE(m_pBossMonster);
	SAFE_RELEASE(m_pMinimapTexture);
}

void CStage::SetMinimapTexture(const string & strKey, const wchar_t * pFileName, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
{
	SAFE_RELEASE(m_pMinimapTexture);
	_SIZE tSize = tRB - tLT;
	SetMinimapSize(tSize);
	m_pMinimapTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, tOrigin, tSize, tLT, tRB);
}

void CStage::AddMonster(CMonster * pMonster)
{
	m_iGen++;
	pMonster->AddRef();
	m_vecMonster.push_back(pMonster);
}

void CStage::SetBossMonster(CMonster * pBossMonster)
{
	pBossMonster->AddRef();
	m_pBossMonster = pBossMonster;
}

bool CStage::Init()
{
	return true;
}

void CStage::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CStage::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);

	/*if (m_iGen > 0)
	{
		m_fMonsterGenTime += fDeltaTime;
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
					pMonster->m_pAnimation->ReturnClip();
					pMonster->SetPhysics(true);
					pMonster->SetEnable(true);
					pMonster->SetDead(false);
				}
			}
		}
	}
	if (m_pBossMonster)
	{
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
				m_pBossMonster->SetDead(false);
			}
		}
	}*/

	return 0;
}

int CStage::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);

	return 0;
}

void CStage::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CStage::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CStage * CStage::Clone()
{
	return new CStage(*this);
}
