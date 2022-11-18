#pragma once
#include "../ActiveSkill.h"
class CShard :
	public CActiveSkill
{
public:
	CShard();
	CShard(const CShard& shard);
	~CShard();

private:
	class CShardBullet* m_pBulletPrototype;
	bool m_bCreateBullet;
	float m_fCreateTime;
	float m_fCreateDelay;
	bool m_bCreateDelay;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CShard* Clone();

public:
	virtual void ActiveSkill(CObj* pObj);
};

