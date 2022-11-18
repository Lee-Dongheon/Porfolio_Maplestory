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
	_SIZE m_tMinimapSize;

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

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CStage* Clone();
};

