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
	if (m_CollisionList.size() < 2) // �浹�� �� �ִ� ������Ʈ�� 1�� > �浹 X
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

	// ����� ����Ʈ ���� > �� ���긶�� ���̾� �� ����ִ� �浹ü�� ������ �־�� �ϱ� ����(AddObject�� ���̾�� obj->GetLife()�� true�� obj�� �־���)
	m_CollisionList.clear();
}

bool CCollisionManager::Collision(CObj * pSrc, CObj * pDest, float fDeltaTime)
{
	bool bCollision = false; // �浹 ����

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

				(*iterDest)->SetHitPoint((*iterSrc)->GetHitPoint()); // �浹������ �����ϹǷ� ����

				// �浹 ��Ͽ��� ������ �浹������ ���ٸ� ó�� �浹�� ��
				if (!(*iterSrc)->CheckCollisionList(*iterDest))
				{
					// ���θ� �浹ü ����Ʈ�� �߰����ش�.
					(*iterSrc)->AddCollider(*iterDest);
					(*iterDest)->AddCollider(*iterSrc);

					(*iterSrc)->CallFunction(CS_ENTER, *iterDest, fDeltaTime);
					(*iterDest)->CallFunction(CS_ENTER, *iterSrc, fDeltaTime);
				}
				// �ƴ϶�� ���� �浹�� �� �ִ� ��� > ��� �浹 ������ ��
				else
				{
					(*iterSrc)->CallFunction(CS_STAY, *iterDest, fDeltaTime);
					(*iterDest)->CallFunction(CS_STAY, *iterSrc, fDeltaTime);
				}
			}
			// �浹�� �ƴ�, �׷��� �浹ü�� ��ϵǾ������� �浹�� ���� ������ ��
			else if ((*iterSrc)->CheckCollisionList(*iterDest))
			{
				// �浹�� �������Ƿ� ��Ͽ��� �����ش�.
				(*iterSrc)->EraseCollisionList(*iterDest);
				(*iterDest)->EraseCollisionList(*iterSrc);

				(*iterSrc)->CallFunction(CS_LEAVE, *iterDest, fDeltaTime);
				(*iterDest)->CallFunction(CS_LEAVE, *iterSrc, fDeltaTime);
			}
		}
	}

	return bCollision;
}
