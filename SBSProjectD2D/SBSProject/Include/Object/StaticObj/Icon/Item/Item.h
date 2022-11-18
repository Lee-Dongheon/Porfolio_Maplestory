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

	bool m_bIsGot; // ������ ȹ�� �÷���

	int m_iNum; // ������ ����, �޼��� ��� �޼ҷ�
	ITEM_TYPE m_eType; // ������ Ÿ��

	int m_iPrice; // ������ ���� (������), �ǸŰ��� �� ������ 50%

	list<class CEquipParts*> m_PartsList; // ���������� ��� ����ϴ� ���� ������Ʈ
	EQUIP_TYPE m_eEquipType; // �������� ���� Ÿ��

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


