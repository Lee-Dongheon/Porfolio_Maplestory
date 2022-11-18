#pragma once
#include "../Game.h"

class CLayer;
class CObj;

class CScene
{
protected:
	friend class CSceneManager;
	friend class CServerManager;
	
	template<typename T>
	friend void Safe_Delete_Map(T&);

protected:
	CScene();
	virtual ~CScene() = 0;

protected:
	list<CLayer*> m_LayerList;
	class CSound* m_pBGM;
	RESOLUTION m_tSceneRS;
	RESOLUTION m_tSceneBackRS;
	string m_strTag;
	CObj* m_pPlayer;
	POSITION m_tDefaultPos;
	float m_fYPosMax;

	int m_iNum; // Scene 고유번호

protected:
	void ReleasePlayer();
	void KillOtherPlayer();
	void SetPlayer(CObj* pObj, int iSceneNum = -1);

public:
	void SetSceneRS(RESOLUTION tRS) { m_tSceneRS = tRS; }
	void SetSceneBackRS(RESOLUTION tRS) { m_tSceneBackRS = tRS; }
	void SetTag(const string& strTag) { m_strTag = strTag; }
	void SetDefaultPos(POSITION& tPos) { m_tDefaultPos = tPos; }
	void SetYPosMax(float y) { m_fYPosMax = y; }
	void SetNum(int iNum) { m_iNum = iNum; }

public:
	RESOLUTION GetSceneRS() const { return m_tSceneRS; }
	RESOLUTION GetSceneBackRS() const { return m_tSceneBackRS; }
	string GetTag() const { return m_strTag; }
	CSound* GetBGM() const { return m_pBGM; }
	float GetYPosMax() const { return m_fYPosMax; }
	int GetNum() const { return m_iNum; }

public:
	CLayer* CreateLayer(const string& strTag, int iZOrder = 0);
	CLayer* CreateUILayer(const string& strTag, int iZOrder = INT_MAX);
	CLayer* FindLayer(const string& strTag);

public:
	void GetObjList(OBJECT_TYPE eType, list<CObj*> & objList, RECTANGLE& tRect);

public:
	virtual bool Init();

	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	

public:
	static bool LayerSort(CLayer* pL1, CLayer* pL2);

public:
	template<typename T>
	static T* CreatePrototype(const string& strTag)
	{
		T* pObj = new T;

		pObj->SetTag(strTag);

		if (!(pObj)->Init())
		{
			SAFE_RELEASE(pObj);
			return NULL;
		}

		pObj->AddRef();
		m_mapPrototype.insert(make_pair(strTag, pObj));

		return pObj;
	}

public:
	template<typename T1, typename T2>
	void AddMonster(T2* pStage, const string& monsterTag, CLayer* pLayer, POSITION tOriginPos)
	{
		T1* pMonster = CObj::CreateObj<T1>(monsterTag, pLayer);
		pMonster->SetOriginPos(tOriginPos);
		pMonster->SetPos(tOriginPos);
		pMonster->SetEnable(true);
		pStage->AddMonster(pMonster);
		SAFE_RELEASE(pMonster);
	}

	template<typename T1, typename T2>
	void AddBossMonster(T2* pStage, const string& monsterTag, CLayer* pLayer, POSITION tOriginPos)
	{
		T1* pMonster = CObj::CreateObj<T1>(monsterTag, pLayer);
		pMonster->SetOriginPos(tOriginPos);
		pMonster->SetPos(tOriginPos);
		pMonster->SetEnable(true);
		pStage->SetBossMonster(pMonster);
		SAFE_RELEASE(pMonster);
	}
};

