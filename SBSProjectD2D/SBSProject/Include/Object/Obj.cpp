#include "Obj.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Layer.h"
#include "../Collider/Collider.h"
#include "../Resources/ResourcesManager.h"
#include "../Collider/CollisionManager.h"
#include "../Resources/Texture.h"
#include "../Core/Camera.h"
#include "../Core/Animation.h"
#include "../Math.h"
#include "../Core/CriticalSectionObj.h"

list<CObj*> CObj::m_ObjList;

CObj::CObj() :
	m_pLayer(NULL),
	m_pScene(NULL),
	m_pTexture(NULL),
	m_pAnimation(NULL),
	m_bIsPhysics(false),
	m_fGravityTime(0.f),
	m_eDir(MD_LEFT),
	m_eObjType(OT_DEFAULT),
	m_pCritic(NULL)
{
	// 임계영역 객체 초기화
	m_pCritic = new CCriticalSectionObj;
}

CObj::CObj(const CObj & obj)
{
	*this = obj;
	
	m_strTag = obj.m_strTag;
	m_eObjType = obj.m_eObjType;

	m_pAnimation = NULL;
	if (obj.m_pAnimation)
	{
		m_pAnimation = obj.m_pAnimation->Clone();
		m_pAnimation->SetObj(this);
	}		
	
	m_fGravityTime = 0.f;
	
	m_pTexture = NULL;
	if (obj.m_pTexture)
	{
		m_pTexture = obj.m_pTexture;
		m_pTexture->AddRef();
	}
	m_eDir = obj.m_eDir;
	
	m_ColliderList.clear();

	list<CCollider*>::const_iterator iter;
	list<CCollider*>::const_iterator iterEnd = obj.m_ColliderList.end();
	
	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; iter++)
	{
		CCollider* pColl = (*iter)->Clone();
	
		pColl->SetObj(this);
	
		m_ColliderList.push_back(pColl);
	}

	m_pCritic = new CCriticalSectionObj;
}

CObj::~CObj()
{
	SAFE_DELETE(m_pCritic);
	Safe_Release_VecList(m_ColliderList);

	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTexture);
}

void CObj::AddObj(CObj * pObj)
{
	pObj->AddRef();
	m_ObjList.push_back(pObj);
}

CObj * CObj::FindObject(const string & strTag)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}

void CObj::EraseObj(CObj * pObj)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if (*iter == pObj)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj(const string & strTag)
{
	list<CObj*>::iterator iter;
	list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		if ((*iter)->GetTag() == strTag)
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			return;
		}
	}
}

void CObj::EraseObj()
{
	Safe_Release_VecList(m_ObjList);
}

void CObj::SetTexture(CTexture * pTexture)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;

	if (pTexture)
		pTexture->AddRef();
}

void CObj::SetTexture(const string & strKey, const wchar_t * pFileName,
	POSITION& tOrigin, POSITION& tLT, POSITION& tRB)
{
	SAFE_RELEASE(m_pTexture);
	_SIZE tSize = tRB - tLT;
	m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, tOrigin, tSize, tLT, tRB);
}

void CObj::SetCriticEnter()
{
	if (m_pCritic)
		m_pCritic->Enter();
}

void CObj::SetCriticLeave()
{
	if (m_pCritic)
		m_pCritic->Leave();
}

void CObj::Input(float fDeltaTime)
{
}

int CObj::Update(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Update(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			iter++;
	}

	if (m_pAnimation)
	{
		bool bChange = m_pAnimation->Update(fDeltaTime);
		if (bChange)
		{
			PANIMATIONCLIP pClip = m_pAnimation->GetCurrentClip();
			SetTexture(pClip->vecTexture[pClip->iFrame]);
		}
	}
	
	return 0;
}

int CObj::LateUpdate(float fDeltaTime)
{
	list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->LateUpdate(fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			iter++;
	}

	return 0;
}

void CObj::Collision(float fDeltaTime)
{
	
}

void CObj::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	if (m_pTexture)
		m_pTexture->Render(m_tPos, pGDIRenderTarget, pRenderTarget, fDeltaTime, 1.f, true, m_eDir);

	/*list<CCollider*>::iterator iter;
	list<CCollider*>::iterator iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->GetEnable())
		{
			iter++;
			continue;
		}

		(*iter)->Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

		if (!(*iter)->GetLife())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
		}
		else
			iter++;
	}*/
}

int CObj::Update(PSC_ObjectPacket pPacket)
{
	m_pCritic->Enter();

	SetObjState(pPacket->objState);
	SetDir(pPacket->eDir);
	SetPos(pPacket->tPos);
	SetEnable(pPacket->enable);

	m_pCritic->Leave();

	return 0;
}

CAnimation * CObj::CreateAnimation(const string & strTag)
{
	SAFE_RELEASE(m_pAnimation);

	m_pAnimation = new CAnimation;

	m_pAnimation->SetTag(strTag);
	m_pAnimation->SetObj(this);

	if (!m_pAnimation->Init())
	{
		SAFE_RELEASE(m_pAnimation);

		return NULL;
	}

	m_pAnimation->AddRef();

	return m_pAnimation;
}

bool CObj::AddAnimationClip(const string & strName, ANIMATION_OPTION eOption, int iFrameMax,
	POSITION* arrOrigin, POSITION* arrLT, POSITION* arrRB, float* arrAnimationDelay,
	const string & strTexKey, const wchar_t * pFilePath)
{
	if (!m_pAnimation)
		return false;

	m_pAnimation->AddClip(strName, eOption, iFrameMax, arrOrigin,
		arrLT, arrRB, arrAnimationDelay, strTexKey, pFilePath);

	return true;
}