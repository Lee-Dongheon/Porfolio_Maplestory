#pragma once
#include "../StaticObj.h"

class CMayaNPC :
	public CStaticObj
{
public:
	CMayaNPC();
	CMayaNPC(const CMayaNPC& obj);
	virtual ~CMayaNPC();

private:
	bool m_bClicked;
	float m_fEyeTime;
	float m_fEyeLimitTime;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CMayaNPC* Clone();

public:
	template<typename T>
	T* AddItem(const string& strTag, ITEM_TYPE eType = IT_ETC)
	{
		T* pItem = new T;

		pItem->SetObj(this);
		pItem->SetTag(strTag);
		pItem->SetItemType(eType);

		if (!pItem->Init())
		{
			SAFE_RELEASE(pItem);
			return NULL;
		}

		pItem->AddRef();
		m_ItemList.push_back(pItem);

		return pItem;
	}
};

