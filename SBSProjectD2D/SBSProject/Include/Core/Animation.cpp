#include "Animation.h"
#include "../Resources/Texture.h"
#include "../Resources/ResourcesManager.h"
#include "../Object/Obj.h"
#include "../Object/MoveObj/MoveObj.h"

CAnimation::CAnimation() :
	m_pCurClip(NULL),
	m_strDefaultClip(""),
	m_strCurClip(""),
	m_bMotionEnd(false),
	m_pObj(NULL)
{
}

CAnimation::CAnimation(const CAnimation & animation)
{
	*this = animation;

	m_mapClip.clear();

	unordered_map<string, PANIMATIONCLIP>::const_iterator iter;
	unordered_map<string, PANIMATIONCLIP>::const_iterator iterEnd = animation.m_mapClip.end();

	for (iter = animation.m_mapClip.begin(); iter != iterEnd; iter++)
	{
		PANIMATIONCLIP pClip = new ANIMATIONCLIP;
		string strName = iter->first;

		*pClip = *iter->second;

		for (size_t i = 0; i < pClip->vecTexture.size(); i++)
		{
			pClip->vecTexture[i]->AddRef();
		}
		m_mapClip.insert(make_pair(strName, pClip));
	}

	m_pCurClip = NULL;
	m_strCurClip = "";
	SetCurrentClip(animation.m_strCurClip);
}

CAnimation::~CAnimation()
{
	unordered_map<string, PANIMATIONCLIP>::iterator iter;
	unordered_map<string, PANIMATIONCLIP>::iterator iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; iter++)
	{
		for (size_t i = 0; i < iter->second->vecTexture.size(); i++)
		{
			SAFE_RELEASE(iter->second->vecTexture[i]);
		}

		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

bool CAnimation::AddClip(const string & strName, ANIMATION_OPTION eOption, int iFrameMax,
	POSITION* arrOrigin, POSITION* arrLT, POSITION* arrRB, float* arrAnimationDelay,
	const string & strTexKey, const wchar_t * pFilePath)
{
	PANIMATIONCLIP pClip = new ANIMATIONCLIP;

	pClip->eOption = eOption;
	pClip->iFrame = 0;
	pClip->iFrameMax = iFrameMax;
	pClip->fAnimationTime = 0.f;

	CTexture* pTex;
	int len = wcslen(pFilePath);
	wchar_t n[7];

	for (int i = 0; i < iFrameMax; i++)
	{
		wchar_t* tempFileName = new wchar_t[len + 7];
		wcscpy(tempFileName, pFilePath);
		swprintf(n, L"%d.png", i);
		wcscat(tempFileName, n);

		string tempTexKey = "";
		tempTexKey = strTexKey + "_" + to_string(i);
		_SIZE tSize = arrRB[i] - arrLT[i];
		pTex = GET_SINGLE(CResourcesManager)->LoadTexture(tempTexKey, tempFileName,
			arrOrigin[i], tSize, arrLT[i], arrRB[i]);

		delete[] tempFileName;
		pClip->vecDelay.push_back(arrAnimationDelay[i]);
		pClip->vecTexture.push_back(pTex);
	}

	m_mapClip.insert(make_pair(strName, pClip));

	if (m_strDefaultClip.empty())
		SetDefaultClip(strName);

	if (m_strCurClip.empty())
		SetCurrentClip(strName);

	return true;
}

void CAnimation::SetCurrentClip(const string & strCurClip)
{
	ChangeClip(strCurClip);
}

void CAnimation::SetDefaultClip(const string & strDefaultClip)
{
	m_strDefaultClip = strDefaultClip;
}

void CAnimation::ChangeClip(const string & strClip)
{
	if (m_strCurClip == strClip)
		return;

	m_strCurClip = strClip;

	if (m_pCurClip)
	{
		m_pCurClip->iFrame = 0;
		m_pCurClip->fAnimationTime = 0.f;
	}

	m_pCurClip = FindClip(strClip);

	if (!m_pCurClip)
		return;

	if(m_pObj)
		m_pObj->SetTexture(m_pCurClip->vecTexture[m_pCurClip->iFrame]);
}

void CAnimation::ReturnClip()
{
	ChangeClip(m_strDefaultClip);
}

PANIMATIONCLIP CAnimation::FindClip(const string & strName)
{
	unordered_map<string, PANIMATIONCLIP>::iterator iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return NULL;

	return iter->second;
}

bool CAnimation::Init()
{
	return true;
}

bool CAnimation::Update(float fDeltaTime)
{
	bool bChange = false;
	m_bMotionEnd = false;

	m_pCurClip->fAnimationTime += fDeltaTime;

	while (m_pCurClip->fAnimationTime >= m_pCurClip->vecDelay[m_pCurClip->iFrame])
	{
		m_pCurClip->fAnimationTime -= m_pCurClip->vecDelay[m_pCurClip->iFrame];
		m_pCurClip->iFrame++;
		bChange = true;

		if (m_pCurClip->iFrame == m_pCurClip->iFrameMax)
		{
			m_pCurClip->iFrame = 0;
			m_bMotionEnd = true;

			switch (m_pCurClip->eOption)
			{
			case AO_ONCE_RETURN:
				ChangeClip(m_strDefaultClip);
				break;
			case AO_ONCE_DISABLE:
				if (m_pObj)
					m_pObj->SetEnable(false);
				break;
			case AO_ONCE_DESTROY:
				if (m_pObj)
					m_pObj->Die();
				break;
			case AO_TIME_RETURN:
				break;
			case AO_TIME_DESTROY:
				break;
			}

			bChange = true;
		}
	}

	return bChange;
}

CAnimation * CAnimation::Clone()
{
	return new CAnimation(*this);
}
