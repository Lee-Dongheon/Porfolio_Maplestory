#pragma once
#include "../Icon.h"

class CItem :
	public CIcon
{
public:
	CItem();
	CItem(const CItem& item);
	virtual ~CItem();

protected:
	class CObj* m_pObj;

	bool m_bIsGot; // 아이템 획득 플래그

	int m_iNum; // 아이템 개수, 메소일 경우 메소량
	ITEM_TYPE m_eType; // 아이템 타입

	int m_iPrice; // 아이템 정가 (상점가), 판매가는 이 가격의 50%

	list<class CEquipParts*> m_PartsList; // 장비아이템인 경우 사용하는 파츠 오브젝트
	EQUIP_TYPE m_eEquipType; // 아이템의 장착 타입

public:
	void SetObj(CObj* pObj)				{ m_pObj = pObj; }
	void SetNum(int n)					{ m_iNum = n; }
	void SetItemType(ITEM_TYPE eType)	{ m_eType = eType; }
	void SetPrice(int iPrice)			{ m_iPrice = iPrice; }
	void SetEquipType(EQUIP_TYPE eType) { m_eEquipType = eType; }

public:
	CObj* GetObj() const			{ return m_pObj; }
	int GetNum() const				{ return m_iNum; }
	ITEM_TYPE GetItemType() const	{ return m_eType; }
	int GetPrice() const			{ return m_iPrice; }
	EQUIP_TYPE GetEquipType() const { return m_eEquipType; }

public:
	void AddNum(int i) { m_iNum += i; }

public:
	CEquipParts* CreateParts(const string& strTag, EQUIP_TYPE eType);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CItem* Clone();

public:
	virtual void UseItem();

public:
	void ItemInit();
};


