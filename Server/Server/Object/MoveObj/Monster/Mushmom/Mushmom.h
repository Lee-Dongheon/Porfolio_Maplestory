#pragma once
#include "../Monster.h"
class CMushmom :
	public CMonster
{
public:
	CMushmom();
	CMushmom(const CMushmom& mushmom);
	virtual ~CMushmom();

private:
	bool m_bIsAttack;

public:
	virtual bool Init();
	//virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CMushmom* Clone();

protected:
	void Attack();

};

