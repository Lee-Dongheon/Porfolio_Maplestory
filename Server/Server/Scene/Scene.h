#pragma once
#include "../server.h"

class CLayer;
class CObj;

class CScene
{
protected:
	friend class CSceneManager;
	friend class CClientManager;

	template<typename T>
	friend void Safe_Delete_Map(T&);

protected:
	CScene();
	virtual ~CScene() = 0;

protected:
	list<CObj*> m_UserList; // Scene에 있는 user list
	list<CObj*> m_ObjList; // Scene에 있는 obj list
	class CCriticalSectionObj* m_pObjListCS; // objlist 관리용 임계영역 오브젝트
	string m_strTag;
	int m_iNum; // scene 고유번호
	POSITION m_tDefaultPos; // 맵 입장시 기본 위치
	RESOLUTION m_tSceneRS;
	float m_fYPosMax;
	float m_fSaveTime;

public:
	void SetTag(const string& strTag) { m_strTag = strTag; }
	void SetNum(int iNum) { m_iNum = iNum; }
	void SetDefaultPos(POSITION& tPos) { m_tDefaultPos = tPos; }
	void SetDefaultPos(float x, float y) { m_tDefaultPos.x = x; m_tDefaultPos.y = y; }
	void SetSceneRS(RESOLUTION tRS) { m_tSceneRS = tRS; }
	void SetYPosMax(float y) { m_fYPosMax = y; }
	void SetCriticEnter();
	void SetCriticLeave();

public:
	RESOLUTION GetSceneRS() const { return m_tSceneRS; }
	string GetTag() const { return m_strTag; }
	int GetNum() const { return m_iNum; }
	POSITION GetDefaultPos() const { return m_tDefaultPos; }
	float GetYPosMax() const { return m_fYPosMax; }

public:
	void AddObject(CObj* pObj);
	void EraseObject(CObj* pObj);

public:
	void AddUser(CObj* pUser);
	void EraseUser(CObj* pUser);

public:
	CObj* FindItem(const string& strTag, POSITION tPos);
	CObj* FindMonster(POSITION tOrigin);

public:
	virtual bool Init();
	void Input(float fDeltaTime);
	int Update(float fDeltaTime);
	int LateUpdate(float fDeltaTime);
	void Collision(float fDeltaTime);
	void BroadCast(float fDeltaTime);

public:
	template<typename T1, typename T2>
	void AddMonster(T2* pStage, const string& monsterTag, CScene* pScene, POSITION tOriginPos)
	{
		T1* pMonster = CObj::CreateObj<T1>(monsterTag, pScene);
		pMonster->SetOriginPos(tOriginPos);
		pMonster->SetPos(tOriginPos);
		pMonster->SetEnable(false);
		pStage->AddMonster(pMonster);
		SAFE_RELEASE(pMonster);
	}

	template<typename T1, typename T2>
	void AddBossMonster(T2* pStage, const string& monsterTag, CScene* pScene, POSITION tOriginPos)
	{
		T1* pMonster = CObj::CreateObj<T1>(monsterTag, pScene);
		pMonster->SetOriginPos(tOriginPos);
		pMonster->SetPos(tOriginPos);
		pMonster->SetEnable(false);
		pStage->SetBossMonster(pMonster);
		SAFE_RELEASE(pMonster);
	}
};