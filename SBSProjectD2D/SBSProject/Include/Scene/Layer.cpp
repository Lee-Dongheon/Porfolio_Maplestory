#include "Layer.h"
#include "../Object/Obj.h"
#include "../Object/StaticObj/Portal/Portal.h"
#include "../Collider/CollisionManager.h"
#include "../Object/MoveObj/Monster/Monster.h"
#include "../Core/CriticalSectionObj.h"

CLayer::CLayer() :
	m_iZOrder(0),
	m_strTag(""),
	m_pScene(NULL),
	m_bEnable(true),
	m_bLife(true),
	m_pCritic(NULL)
{
	m_pCritic = new CCriticalSectionObj;
}

CLayer::~CLayer()
{
	m_pCritic->Enter();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		CObj::EraseObj(*iter);
		SAFE_RELEASE((*iter));
	}
	m_pCritic->Leave();

	m_ObjList.clear();

	SAFE_DELETE(m_pCritic);
}

void CLayer::AddObject(CObj * pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();
	m_pCritic->Enter();
	m_ObjList.push_back(pObj);
	m_pCritic->Leave();
}

void CLayer::EraseObject(CObj * pObj)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter) == pObj)
		{
			SAFE_RELEASE((*iter));
			m_ObjList.erase(iter);
			break;
		}
	}
	m_pCritic->Leave();
}

POSITION CLayer::GetPortalPos(int iSceneNum)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag().find("Portal") != string::npos)
		{
			CPortal* pPortal = (CPortal*)(*iter);
			if (pPortal->GetSceneNum() == iSceneNum)
			{
				m_pCritic->Leave();
				return pPortal->GetPos();
			}
		}
	}
	m_pCritic->Leave();

	return POSITION(0.f, 0.f);
}

CObj * CLayer::FindStage()
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetObjType() == OT_MAP)
		{
			(*iter)->AddRef();
			m_pCritic->Leave();
			return (*iter);
		}
	}
	m_pCritic->Leave();

	return NULL;
}

CObj * CLayer::FindMonster(POSITION tOrigin)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetObjType() == OT_MONSTER)
		{
			if (((CMonster*)(*iter))->GetOriginPos() == tOrigin)
			{
				(*iter)->AddRef();
				m_pCritic->Leave();
				return (*iter);
			}
		}
	}
	m_pCritic->Leave();

	return NULL;
}

CObj * CLayer::FindItem(const string & strTag, POSITION tPos)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if (((*iter)->GetTag() == strTag) &&
			((*iter)->GetPos() == tPos))
		{
			(*iter)->AddRef();
			m_pCritic->Leave();
			return (*iter);
		}
	}
	m_pCritic->Leave();

	return NULL;
}

void CLayer::KillOtherPlayer(CObj * pPlayer)
{
	// pPlayer와 다른 OT_PLAYER형의 오브젝트
	// == OtherPlayer 지우기 함수

	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetObjType() == OT_OTHERPLAYER &&
			(*iter) != pPlayer)
		{
			CObj::EraseObj((*iter));
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	m_pCritic->Leave();
}

void CLayer::Input(float fDeltaTime)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Input(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	m_pCritic->Leave();
}

int CLayer::Update(float fDeltaTime)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	m_pCritic->Leave();
	return 0;
}

int CLayer::LateUpdate(float fDeltaTime)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	m_pCritic->Leave();
	return 0;
}

void CLayer::Collision(float fDeltaTime)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
		{
			GET_SINGLE(CCollisionManager)->AddObject(*iter);

			iter++;
		}
	}
	m_pCritic->Leave();
}

void CLayer::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	m_pCritic->Enter();
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();	

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

		if (!(*iter)->GetLife())
		{
			CObj::EraseObj(*iter);
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	m_pCritic->Leave();
}