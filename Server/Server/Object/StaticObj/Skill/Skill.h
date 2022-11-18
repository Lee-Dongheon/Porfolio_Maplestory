#pragma once
#include "../StaticObj.h"

class CSkill :
	public CStaticObj
{
public:
	CSkill();
	CSkill(const CSkill& obj);
	virtual ~CSkill();

private:
	int m_iStepLevel; // ��ų ����
	int m_iLevel; // ��ų ����

public:
	void SetSkillStep(int iStep) { m_iStepLevel = iStep; }
	void SetLevel(int iLevel) { m_iLevel = iLevel; }

public:
	int GetSkillStep() const { return m_iStepLevel; }
	int GetLevel() const { return m_iLevel; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CSkill* Clone();
};

