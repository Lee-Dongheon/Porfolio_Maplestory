#pragma once
#include "../StaticObj.h"

class CStage :
	public CStaticObj
{
protected:
	friend class CObj;
	friend class CScene;

protected:
	CStage();
	CStage(const CStage& stage);
	virtual ~CStage();

protected:
	POSITION hw;
	float m_fMonsterGenTime;
	float m_fMonsterGenLimitTime;
	float m_fBossMonsterGenTime;
	float m_fBossMonsterGenLimitTime;
	
	int m_iGen;
	vector<class CMonster*> m_vecMonster;
	class CMonster* m_pBossMonster;
	CTexture* m_pMinimapTexture;
	_SIZE m_tMinimapSize;

protected:
	void SetMinimapTexture(const string & strKey, const wchar_t * pFileName,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	void SetMinimapSize(_SIZE tSize) { m_tMinimapSize = tSize; }

public:
	void Sethw(float x, float y) { hw.x = x; hw.y = y; }
	void AddhwX(float x) { hw.x += x; }
	void AddhwY(float y) { hw.y += y; }

public:
	void AddMonster(class CMonster* pMonster);

public:
	void SetBossMonster(class CMonster* pBossMonster);

public:
	int GetGen() const { return m_iGen; }
	CTexture* GetMinimapTexture() const { return m_pMinimapTexture; }
	_SIZE GetMinimapSize() const { return m_tMinimapSize; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CStage* Clone();
};

