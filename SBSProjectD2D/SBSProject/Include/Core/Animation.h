#pragma once
#include "Ref.h"

class CAnimation :
	public CRef
{
private:
	friend class CObj;
	friend class CActiveSkill;

private:
	CAnimation();
	CAnimation(const CAnimation& animation);
	~CAnimation();

private:
	unordered_map<string, PANIMATIONCLIP> m_mapClip;
	PANIMATIONCLIP m_pCurClip;
	string m_strCurClip;
	string m_strDefaultClip;
	class CObj* m_pObj;
	bool m_bMotionEnd;

public:
	PANIMATIONCLIP GetCurrentClip() const { return m_pCurClip; }
	bool GetMotionEnd() const { return m_bMotionEnd; }

public:
	bool AddClip(const string & strName, ANIMATION_OPTION eOption, int iFrameMax,
		POSITION* arrOrigin, POSITION* arrLT, POSITION* arrRB, float* arrAnimationDelay,
		const string & strTexKey, const wchar_t * pFilePath);

public:
	void SetObj(class CObj* pObj) { m_pObj = pObj; }

	void SetCurrentClip(const string& strCurClip);
	void SetDefaultClip(const string& strDefaultClip);
	void ChangeClip(const string& strClip);
	void ReturnClip();

private:
	PANIMATIONCLIP FindClip(const string& strName);

public:
	bool Init();
	bool Update(float fDeltaTime);
	CAnimation* Clone();
};

