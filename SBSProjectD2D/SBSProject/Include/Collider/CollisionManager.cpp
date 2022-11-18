#include "CollisionManager.h"
#include "../Object/Obj.h"
#include "Collider.h"

DEF_SINGLE(CCollisionManager);

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::AddObject(CObj * pObj)
{
	if (pObj->CheckCollider())
	{
		m_CollisionList.push_back(pObj);
	}
}

void CCollisionManager::Collision(float fDeltaTime)
{
	if (m_CollisionList.size() < 2) // 충돌할 수 있는 오브젝트가 1개 > 충돌 X
	{
		m_CollisionList.clear();
		return;
	}

	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; iter++)
	{
		list<CObj*>::iterator iter1 = iter;
		iter1++;
		list<CObj*>::iterator  iter1End = m_CollisionList.end();
		for (; iter1 != iter1End; iter1++)
		{
			if ((*iter)->GetObjType() == (*iter1)->GetObjType())
				continue;
			Collision(*iter, *iter1, fDeltaTime);
		}
	}

	// 종료시 리스트 비우기 > 매 연산마다 레이어 속 살아있는 충돌체만 가지고 있어야 하기 때문(AddObject는 레이어에서 obj->GetLife()가 true인 obj만 넣어줌)
	m_CollisionList.clear();
}

bool CCollisionManager::Collision(CObj * pSrc, CObj * pDest, float fDeltaTime)
{
	bool bCollision = false; // 충돌 여부

	const list<CCollider*>* pSrcList = pSrc->GetColliderList();
	const list<CCollider*>* pDestList = pDest->GetColliderList();

	list<CCollider*>::const_iterator iterSrc;
	list<CCollider*>::const_iterator iterSrcEnd = pSrcList->end();

	list<CCollider*>::const_iterator iterDest;
	list<CCollider*>::const_iterator iterDestEnd = pDestList->end();

	for (iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; iterSrc++)
	{
		for (iterDest = pDestList->begin(); iterDest != iterDestEnd; iterDest++)
		{
			if ((*iterSrc)->Collision(*iterDest))
			{
				bCollision = true;

				(*iterDest)->SetHitPoint((*iterSrc)->GetHitPoint()); // 충돌지점은 동일하므로 공유

				// 충돌 목록에서 이전에 충돌된적이 없다면 처음 충돌한 것
				if (!(*iterSrc)->CheckCollisionList(*iterDest))
				{
					// 서로를 충돌체 리스트에 추가해준다.
					(*iterSrc)->AddCollider(*iterDest);
					(*iterDest)->AddCollider(*iterSrc);

					(*iterSrc)->CallFunction(CS_ENTER, *iterDest, fDeltaTime);
					(*iterDest)->CallFunction(CS_ENTER, *iterSrc, fDeltaTime);
				}
				// 아니라면 기존 충돌된 적 있는 경우 > 계속 충돌 상태인 것
				else
				{
					(*iterSrc)->CallFunction(CS_STAY, *iterDest, fDeltaTime);
					(*iterDest)->CallFunction(CS_STAY, *iterSrc, fDeltaTime);
				}
			}
			// 충돌이 아님, 그러나 충돌체가 등록되어있으면 충돌이 끝난 직후인 것
			else if ((*iterSrc)->CheckCollisionList(*iterDest))
			{
				// 충돌이 끝났으므로 목록에서 지워준다.
				(*iterSrc)->EraseCollisionList(*iterDest);
				(*iterDest)->EraseCollisionList(*iterSrc);

				(*iterSrc)->CallFunction(CS_LEAVE, *iterDest, fDeltaTime);
				(*iterDest)->CallFunction(CS_LEAVE, *iterSrc, fDeltaTime);
			}
		}
	}

	return bCollision;
}
