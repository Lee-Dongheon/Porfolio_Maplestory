#pragma once
#include "UI.h"
class CUIKey :
	public CUI
{
private:
	friend class CObj;

protected:
	CUIKey();
	CUIKey(const CUIKey& ui);
	virtual ~CUIKey();

private:
	map<string, POSITION> m_mapKeyBox; // Ű ����â������ ��ġ
	// Ű�������� ��� ����� ������ �������� ���� �� �ִ�
	// > "Extra"������ ���� (Ű ����â�� �÷������� ������)

private:
	POSITION FindKeyPos(const string& strTag);
	string FindBoxTag(POSITION tPos);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIKey* Clone();

public:
	void KeyBoxInit();
};

