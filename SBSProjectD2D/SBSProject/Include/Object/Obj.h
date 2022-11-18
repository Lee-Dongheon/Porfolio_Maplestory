#pragma once

#include "../Core/Ref.h"
#include "../Collider/Collider.h"
#include "../Core/Animation.h"

class CTexture;
class CScene;
class CLayer;

class CObj :
	public CRef
{
protected:
	friend class CScene;
	friend class CActiveSkill;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj();

protected:
	static list<CObj*> m_ObjList;

public:
	static void AddObj(CObj* pObj);
	static CObj* FindObject(const string& strTag);
	static void EraseObj(CObj* pObj);
	static void EraseObj(const string& strTag);
	static void EraseObj();

protected:
	CScene* m_pScene; // 현재 object를 가지고 있는 scene
	CLayer* m_pLayer; // 현재 object를 가지고 있는 layer

	bool m_bIsPhysics; // 물리효과 적용 받을지 여부
	float m_fGravityTime; // 중력 적용에 사용하는 float 변수

	OBJECT_TYPE m_eObjType; // 오브젝트 타입

	POSITION m_tPos; // object 좌표
	_SIZE m_tSize; // object 크기
	POSITION m_tPivot; // object pivot
	MOVE_DIR m_eDir; // object의 방향
	
	list<CCollider*> m_ColliderList; // Object Collider List
	CTexture* m_pTexture; // object texture
	CAnimation* m_pAnimation; // Object Animation

	OBJECT_STATE m_eObjState; // object 상태

	class CCriticalSectionObj* m_pCritic;

public:
	void SetScene(CScene* pScene)			{ m_pScene = pScene; }
	void SetLayer(CLayer* pLayer)			{ m_pLayer = pLayer; }
	void SetPhysics(bool bPhysics)			{ m_bIsPhysics = bPhysics; }
	void SetObjType(OBJECT_TYPE eObjType)	{ m_eObjType = eObjType; }
	void SetDir(MOVE_DIR eDir)				{ m_eDir = eDir; }

	void SetPos(const POSITION& tPos)		{ m_tPos = tPos; }
	void SetPos(float x, float y)			{ m_tPos.x = x; m_tPos.y = y; }
	void SetSize(const _SIZE& tSize)			{ m_tSize = tSize; }
	void SetSize(float x, float y)			{ m_tSize.x = x; m_tSize.y = y; }
	void SetPivot(const POSITION& tPivot)	{ m_tPivot = tPivot; }
	void SetPivot(float x, float y)			{ m_tPivot.x = x; m_tPivot.y = y; }

	void SetTexture(CTexture* pTexture);
	void SetTexture(const string & strKey, const wchar_t * pFileName,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);

	void SetObjState(OBJECT_STATE eObjState) { m_eObjState = eObjState; }

	void SetCriticEnter();
	void SetCriticLeave();

public:
	CScene* GetScene() const						{ return m_pScene; }
	CLayer* GetLayer() const						{ return m_pLayer; }
	bool GetPhysics() const							{ return m_bIsPhysics; }
	const list<CCollider*>* GetColliderList() const { return &m_ColliderList; }
	OBJECT_TYPE GetObjType() const					{ return m_eObjType; }
	MOVE_DIR GetDir() const							{ return m_eDir; }

	POSITION GetPos() const		{ return m_tPos; }
	_SIZE GetSize() const		{ return m_tSize; }
	POSITION GetPivot() const	{ return m_tPivot; }

	OBJECT_STATE GetObjState() const { return m_eObjState; }

public:
	void ClearGravity()		{ m_fGravityTime = 0.f; }
	bool CheckCollider()	{ return !m_ColliderList.empty(); }

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CObj* Clone() = 0;
	// object는 objectpacket으로만 업데이트 되므로 매개변수 타입 고정 형변환
	virtual int Update(PSC_ObjectPacket pPacket);

public:
	CAnimation* CreateAnimation(const string& strTag);
	bool AddAnimationClip(const string & strName, ANIMATION_OPTION eOption, int iFrameMax,
		POSITION* arrOrigin, POSITION* arrLT, POSITION* arrRB, float* arrAnimationDelay,
		const string & strTexKey, const wchar_t * pFilePath);

	template<typename T>
	void AddCollisionFunction(const string& strTag, COLLISION_STATE eState, T* pObj, void(T::*pFunc)(CCollider*, CCollider*, float))
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; iter++)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddCollisionFunction(eState, pObj, pFunc);
				break;
			}
		}
	}

	template<typename T>
	T* AddCollider(const string& strTag)
	{
		T* pCollider = new T;

		pCollider->SetObj(this);
		pCollider->SetTag(strTag);

		if (!pCollider->Init())
		{
			SAFE_RELEASE(pCollider);
			return NULL;
		}

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}

	template<typename T>
	static T* CreateObj(const string& strTag, CLayer* pLayer = NULL)
	{
		T* pObj = new T;

		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return NULL;
		}
		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}
		AddObj(pObj);

		return pObj;
	}
};

