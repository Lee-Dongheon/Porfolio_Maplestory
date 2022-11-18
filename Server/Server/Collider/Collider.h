#pragma once
#include "../Core/Ref.h"

class CObj;

class CCollider :
	public CRef
{
protected:
	friend class CObj;

protected:
	CCollider();
	CCollider(const CCollider& coll);
	virtual ~CCollider();

protected:
	COLLIDER_TYPE m_eCollType; // �浹ü Ÿ��
	CObj* m_pObj; // �������ִ� object
	list<CCollider*> m_CollisionList; // üũ�� �浹ü ����Ʈ
									  // �浹�Լ� ������ ����Ʈ
									  // �浹ü Ÿ�Ժ��� ��� �浹ü���� �浹 �Լ��� �����������Ƿ�
									  // ����Ʈ[�浹ü Ÿ��] = {��� �浹ü Ÿ�԰��� �浹�Լ�}
	list<function<void(CCollider*, CCollider*, float)>> m_FuncList[CS_END];
	POSITION m_tHitPoint;

public:
	POSITION GetHitPoint() const { return m_tHitPoint; }
	COLLIDER_TYPE GetColliderType() const { return m_eCollType; }
	CObj* GetObj() const { return m_pObj; }

public:
	void SetHitPoint(const POSITION& tPos) { m_tHitPoint = tPos; }
	void SetObj(CObj* pObj) { m_pObj = pObj; }

public:
	void AddCollisionFunction(COLLISION_STATE eState, void(*pFunc)(CCollider*, CCollider*, float))
	{
		function<void(CCollider*, CCollider*, float)> func;

		func = bind(pFunc, placeholders::_1, placeholders::_2, placeholders::_3);

		m_FuncList[eState].push_back(func);
	}

	template<typename T>
	void AddCollisionFunction(COLLISION_STATE eState, T* pObj, void(T::*pFunc)(CCollider*, CCollider*, float))
	{
		function<void(CCollider*, CCollider*, float)> func;

		func = bind(pFunc, pObj, placeholders::_1, placeholders::_2, placeholders::_3);

		m_FuncList[eState].push_back(func);
	}

	void CallFunction(COLLISION_STATE eState, CCollider* pDest, float fDeltaTime)
	{
		list<function<void(CCollider*, CCollider*, float)>>::iterator iter;
		list<function<void(CCollider*, CCollider*, float)>>::iterator iterEnd = m_FuncList[eState].end();

		for (iter = m_FuncList[eState].begin(); iter != iterEnd; iter++)
		{
			(*iter)(this, pDest, fDeltaTime);
		}
	}

public:
	void AddCollider(CCollider* pCollider) { m_CollisionList.push_back(pCollider); }
	bool CheckCollisionList(CCollider* pCollider)
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; iter++)
		{
			if (*iter == pCollider)
				return true;
		}
		return false;
	}

	void EraseCollisionList(CCollider* pCollider)
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_CollisionList.end();

		for (iter = m_CollisionList.begin(); iter != iterEnd; iter++)
		{
			if (*iter == pCollider)
			{
				m_CollisionList.erase(iter);
				break;
			}
		}
	}

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(CCollider* pDest);
	virtual CCollider* Clone() = 0;

protected:
	bool CollisionRectToRect(const RECTANGLE& src, const RECTANGLE& dest);
	bool CollisionRectToPoint(const RECTANGLE& src, const POSITION& dest);
	bool CollisionRectToPixel(const RECTANGLE & src, const POSITION& tPixelPos, const vector<PIXEL>& vecPixel, int iWidth, int iHeight, PIXEL& rgb);
};