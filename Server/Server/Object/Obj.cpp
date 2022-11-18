#include "Obj.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Collider/Collider.h"
#include "../Collider/CollisionManager.h"
#include "../Core/CriticalSectionObj.h"

CObj::CObj() :
	m_pScene(NULL),
	m_eDir(MD_LEFT),
	m_eObjState(OBJ_STATE_DEFAULT),
	m_eObjType(OT_DEFAULT),
	m_pCritic(NULL)
{
	m_pCritic = new CCriticalSectionObj;
}

CObj::CObj(const CObj & obj)
{
	*this = obj;

	m_strTag = obj.m_strTag;

	m_ColliderList.clear();

	m_eDir = obj.m_eDir;
	list<CCollider*>::const_iterator iter;
	list<CCollider*>::const_iterator iterEnd = obj.m_ColliderList.end();

	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; iter++)
	{
		CCollider* pColl = (*iter)->Clone();

		pColl->SetObj(this);

		m_ColliderList.push_back(pColl);
	}
}

CObj::~CObj()
{
	Safe_Release_VecList(m_ColliderList);
	SAFE_DELETE(m_pCritic);
}

void CObj::SetScene(CScene * pScene)
{
	if (pScene == m_pScene)
		return;

	CScene* pBeforeScene = m_pScene;
	m_pScene = pScene;
	
	if (pBeforeScene)
	{
		pBeforeScene->EraseObject(this);
		if (m_eObjType == OT_PLAYER)
			pBeforeScene->EraseUser(this);
	}
}

void CObj::SetScene(int iSceneNum)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->FindScene(iSceneNum);
	if (!pScene)
	{
		printf("Find scene error!\n");
		return;
	}
	pScene->AddObject(this);
}

void CObj::SetCriticEnter()
{
	if (m_pCritic)
		m_pCritic->Enter();
}

void CObj::SetCriticLeave()
{
	if (m_pCritic)
		m_pCritic->Leave();
}

//
//void CObj::ChangeScene(CScene * pScene)
//{
//	// 현재 scene과 다르다면 바꿔주기
//	// 바꿀때는 기존 scene 의 object layer에서 지워주고 새로운 scene에 추가
//	if (pScene != m_pScene)
//	{
//		if (m_pLayer)
//			m_pLayer->EraseObject(this);
//		m_pScene = pScene;
//		pScene->FindLayer("Object")->AddObject(this);
//	}
//}

void CObj::Input(float fDeltaTime)
{
}

int CObj::Update(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			iter++;
	}

	return 0;
}

int CObj::LateUpdate(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			iter++;
	}

	return 0;
}

void CObj::Collision(float fDeltaTime)
{

}
//
//CObj * CObj::CreateCloneObj(const string & strPrototypeKey, const string & strTag, CLayer * pLayer)
//{
//	CObj* pProto = CScene::FindPrototype(strPrototypeKey);
//
//	if (!pProto)
//		return NULL;
//
//	CObj* pObj = pProto->Clone();
//
//	pObj->SetTag(strTag);
//
//	if (!pObj->Init())
//	{
//		SAFE_RELEASE(pObj);
//		return nullptr;
//	}
//
//	if (pLayer)
//	{
//		pLayer->AddObject(pObj);
//	}
//
//	AddObj(pObj);
//
//	return pObj;
//}
