#include "Portal.h"
#include "../../../Collider/ColliderRect.h"
#include "../../../Scene/SceneManager.h"
#include "../../../Core/Input.h"
#include "../../../Core/Camera.h"
#include "../../../Core/ServerManager.h"
#include "../../../Scene/Scene.h"

CPortal::CPortal():
	m_iSceneNum(1)
{
}

CPortal::CPortal(const CPortal & portal):
	CStaticObj(portal)
{
	m_iSceneNum = portal.m_iSceneNum;
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	SetSize(80.f, 100.f);
	SetPivot(0.5f, 1.f);
	SetObjType(OT_PORTAL);

	CColliderRect* pRC = AddCollider<CColliderRect>("Portal");
	pRC->SetRect(-40.f, -100.f, 40.f, 0.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &CPortal::ChangeScene);
	pRC->AddCollisionFunction(CS_STAY, this, &CPortal::ChangeScene);
	SAFE_RELEASE(pRC);

	CAnimation* pAni = CreateAnimation("Portal");
	SAFE_RELEASE(pAni);

	POSITION arrOrigin[8] = { POSITION(49.f, 134.f), POSITION(48.f, 130.f),
							POSITION(50.f, 123.f), POSITION(50.f, 125.f), 
							POSITION(50.f, 130.f), POSITION(51.f, 130.f), 
							POSITION(49.f, 126.f), POSITION(49.f, 130.f) };
	POSITION arrLT[8] = { POSITION(-49.f, -134.f), POSITION(-48.f, -130.f),
							POSITION(-50.f, -123.f), POSITION(-50.f, -125.f),
							POSITION(-50.f, -130.f), POSITION(-51.f, -130.f),
							POSITION(-49.f, -126.f), POSITION(-49.f, -130.f) };
	POSITION arrRB[8] = { POSITION(52.f, 8.f), POSITION(51.f, 8.f), 
							POSITION(53.f, 8.f), POSITION(53.f, 8.f), 
							POSITION(53.f, 8.f), POSITION(53.f, 8.f), 
							POSITION(52.f, 8.f), POSITION(53.f, 8.f) };
	float arrDelay[8];
	fill_n(arrDelay, 8, 0.1f);

	AddAnimationClip("Portal", AO_LOOP, 8, arrOrigin, arrLT, arrRB, arrDelay,
		"Portal", L"Map/Portal/");

	//_SIZE arrSize[8] = { _SIZE(101.f, 142.f), _SIZE(99.f, 138.f),
	//					_SIZE(103.f, 131.f), _SIZE(103.f, 133.f), 
	//					_SIZE(103, 138.f), _SIZE(104.f, 138.f), 
	//					_SIZE(101.f, 134.f), _SIZE(102.f, 138.f) };

	return true;
}

void CPortal::Input(float fDeltaTime)
{
	CStaticObj::Input(fDeltaTime);
}

int CPortal::Update(float fDeltaTime)
{
	CStaticObj::Update(fDeltaTime);
	
	return 0;
}

int CPortal::LateUpdate(float fDeltaTime)
{
	CStaticObj::LateUpdate(fDeltaTime);
	return 0;
}

void CPortal::Collision(float fDeltaTime)
{
	CStaticObj::Collision(fDeltaTime);
}

void CPortal::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CStaticObj::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);
}

CPortal * CPortal::Clone()
{
	return new CPortal(*this);
}

void CPortal::ChangeScene(CCollider * pSrc, CCollider * pDest, float fDeltaTime)
{
	if (pDest->GetObj()->GetObjType() == OT_PLAYER
		&& KEYPRESS("Up") && KEYDOWN("Up")
		&& (pDest->GetObj()->GetPos().x > m_tPos.x - m_tSize.x * m_tPivot.x)
		&& (pDest->GetObj()->GetPos().x < m_tPos.x + m_tSize.x * m_tPivot.x))
	{
		int iSceneFrom = pDest->GetObj()->GetScene()->GetNum();
		GET_SINGLE(CServerManager)->SendPortal(iSceneFrom, m_iSceneNum);
		GET_SINGLE(CSceneManager)->SetSceneToChange(m_iSceneNum);
	}
}
