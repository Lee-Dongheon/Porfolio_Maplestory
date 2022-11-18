#pragma once

#include "../Game.h"

class CObj;
class CScene;

class CLayer
{
private:
	friend class CScene;
	friend class CSceneManager;

protected:
	CLayer();

public:
	virtual ~CLayer();

protected:
	CScene* m_pScene;
	string m_strTag; // layer �̸�
	int m_iZOrder; // layer Z ��ġ
	list<CObj*> m_ObjList; // layer�� ����ִ� Objlist
	bool m_bEnable; // layer Ȱ������
	bool m_bLife; // layer ����
	class CCriticalSectionObj* m_pCritic;

public:
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetTag(const string& strTag) { m_strTag = strTag; }
	void SetZOrder(int iZOrder) { m_iZOrder = iZOrder; }
	void SetScene(CScene* pScene) { m_pScene = pScene; }

public:
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }
	int GetZOrder() const { return m_iZOrder; }
	string GetTag() const { return m_strTag; }
	CScene* GetScene() const { return m_pScene; }

public:
	void Die() { m_bLife = false; }
	void AddObject(CObj* pObj);
	void EraseObject(CObj* pObj);
	POSITION GetPortalPos(int iSceneNum);
	CObj* FindStage();
	CObj* FindMonster(POSITION tOrigin);
	CObj* FindItem(const string& strTag, POSITION tPos);

public:
	void KillOtherPlayer(CObj* pPlayer);

public:
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
};

