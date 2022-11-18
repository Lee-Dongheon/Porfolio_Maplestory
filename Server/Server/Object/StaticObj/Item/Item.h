#pragma once
#include "../StaticObj.h"

class CItem :
	public CStaticObj
{
public:
	CItem();
	CItem(const CItem& obj);
	virtual ~CItem();

private:
	int m_iNum; // 아이템 개수
	ITEM_TYPE m_eItemType; // 아이템 분류
	float m_fThrowTime; // 아이템 버려진 시간
	POSITION m_tCoord; // 아이템 창에서의 좌표
	EQUIP_TYPE m_eEquipType; // 장비인 경우, 장비 종류

public:
	void SetNum(int iNum) { m_iNum = iNum; }
	void SetItemType(ITEM_TYPE eType) { m_eItemType = eType; }
	void SetCoord(POSITION tCoord) { m_tCoord = tCoord; }
	void SetEquipType(EQUIP_TYPE eType) { m_eEquipType = eType; }

public:
	int GetNum() const { return m_iNum; }
	ITEM_TYPE GetItemType() const { return m_eItemType; }
	POSITION GetCoord() const { return m_tCoord; }
	EQUIP_TYPE GetEquipType()const { return m_eEquipType; }

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	//virtual void Collision(float fDeltaTime);
	virtual CItem* Clone();
};

