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
	int m_iNum; // ������ ����
	ITEM_TYPE m_eItemType; // ������ �з�
	float m_fThrowTime; // ������ ������ �ð�
	POSITION m_tCoord; // ������ â������ ��ǥ
	EQUIP_TYPE m_eEquipType; // ����� ���, ��� ����

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

