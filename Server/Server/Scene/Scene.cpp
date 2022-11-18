#include "Scene.h"
#include "../Object/Obj.h"
#include "../Collider/CollisionManager.h"
#include "../Core/ClientManager.h"
#include "../Core/CriticalSectionObj.h"
#include "../Core/DBManager.h"
#include "../Object/MoveObj/Monster/Monster.h"

//unordered_map<string, CObj*> CScene::m_mapPrototype;

CScene::CScene():
	m_pObjListCS(NULL),
	m_fSaveTime(0.f)
{
	m_ObjList.clear();
	m_UserList.clear();

	m_pObjListCS = new CCriticalSectionObj;
}

CScene::~CScene()
{
	SAFE_DELETE(m_pObjListCS);
	Safe_Release_VecList(m_ObjList);
	Safe_Release_VecList(m_UserList);
}

void CScene::SetCriticEnter()
{
	if (m_pObjListCS)
		m_pObjListCS->Enter();
}

void CScene::SetCriticLeave()
{
	if (m_pObjListCS)
		m_pObjListCS->Leave();
}

void CScene::AddObject(CObj * pObj)
{
	SetCriticEnter();

	pObj->AddRef();
	m_ObjList.push_back(pObj);

	SetCriticLeave();

	pObj->SetScene(this);
	if (pObj->GetObjType() == OT_PLAYER)
		AddUser(pObj);
}

void CScene::EraseObject(CObj * pObj)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter) == pObj)
		{
			GET_SINGLE(CClientManager)->DeleteObjectBroadcast((*iter));
			SAFE_RELEASE((*iter));
			m_ObjList.erase(iter);
			break;
		}
	}
}

void CScene::AddUser(CObj * pUser)
{
	pUser->AddRef();

	m_UserList.push_back(pUser);
}

void CScene::EraseUser(CObj * pUser)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_UserList.end();

	for (iter = m_UserList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter) == pUser)
		{
			SAFE_RELEASE(pUser);
			m_UserList.erase(iter);
			break;
		}
	}
}

CObj* CScene::FindItem(const string & strTag, POSITION tPos)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if (((*iter)->GetTag() == strTag) &&
			((*iter)->GetPos() == tPos))
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}

	return NULL;
}

CObj * CScene::FindMonster(POSITION tOrigin)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if (((CMonster*)(*iter))->GetOriginPos() == tOrigin)
		{
			(*iter)->AddRef();
			return (*iter);
		}
	}

	return NULL;
}

bool CScene::Init()
{
	return true;
}

void CScene::Input(float fDeltaTime)
{
	SetCriticEnter();

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
			GET_SINGLE(CClientManager)->DeleteObjectBroadcast((*iter));
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}

	SetCriticLeave();
}

int CScene::Update(float fDeltaTime)
{
	SetCriticEnter();

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
			GET_SINGLE(CClientManager)->DeleteObjectBroadcast((*iter));
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	SetCriticLeave();

	return 0;
}

int CScene::LateUpdate(float fDeltaTime)
{
	SetCriticEnter();

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
			GET_SINGLE(CClientManager)->DeleteObjectBroadcast((*iter));
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
			iter++;
	}
	SetCriticLeave();

	return 0;
}

void CScene::Collision(float fDeltaTime)
{
	SetCriticEnter();

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		//(*iter)->Collision(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			GET_SINGLE(CClientManager)->DeleteObjectBroadcast((*iter));
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}
		else
		{
			GET_SINGLE(CCollisionManager)->AddObject((*iter));

			iter++;
		}
	}
	SetCriticLeave();

	GET_SINGLE(CCollisionManager)->Collision(fDeltaTime);
}

void CScene::BroadCast(float fDeltaTime)
{
	m_fSaveTime += fDeltaTime;

	SetCriticEnter();
		
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_UserList.end();

	for (iter = m_UserList.begin(); iter != iterEnd; iter++)
	{
		if (m_fSaveTime >= 1.f)
		{
			m_fSaveTime -= 1.f;
			GET_SINGLE(CDBManager)->SaveCharacter((*iter));
		}

		GET_SINGLE(CClientManager)->BroadCast((*iter), m_ObjList);		
	}

	SetCriticLeave();
}