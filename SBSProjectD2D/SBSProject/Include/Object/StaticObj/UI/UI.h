#pragma once
#include "../StaticObj.h"
#include "../../../Resources/ResourcesManager.h"
#include "../../../Resources/Texture.h"
#include "../Icon/Icon.h"
#include "../Icon/KeyIcon/KeyIcon.h"
#include "../Tab/Tab.h"
#include "../../MoveObj/Player/Player.h"

class CUI :
	public CStaticObj
{
private:
	friend class CObj;
	friend class CUILayer;

protected:
	CUI();
	CUI(const CUI& ui);
	virtual ~CUI();

protected:
	bool m_bButtonClicked;

protected:
	static map<UI_TYPE, CUI*> m_mapUI;
	static UI_TYPE m_eLastUIType; // ��� UI �߿� ���������� Ȱ��ȭ�� UI
	static CIcon* m_pSelectedIcon; // ���콺�� ���õ� ������
	static bool m_bHaveSelectedIcon;
	static map<string, CTexture*> m_mapKeyImage; // Ű ����â ���� �̹���
	static map<string, CIcon*> m_mapIcon; // Ű �̸��� ������ ��

protected:
	CPlayer* m_pPlayer;
	UI_TYPE m_eUIType;

	list<CTexture*> m_UITextureList; // UI�� ���� texture
	list<CButton*> m_ButtonList;
	unordered_map<string, CTab*> m_mapTab;
	map<string, POSITION> m_mapPos; // texture�� button���� ��ġ�� ������ ��

	string m_strSelectedTab;
	int m_iZOrder;
	bool m_bIsSelected;
	POSITION m_tHeadSize; // â�� �Ӹ��κ� ������

	string m_strLastTab; // Ȱ��ȭ�� tab
	string m_strBeforeTab; // �������� Ȱ��ȭ�� tab
	bool m_bTabChanged; // tab ��ȭ

	UI_TYPE m_eBeforeUIType; // �ڽ� ������ Ȱ��ȭ�� ui
	
public:
	void SetBeforeUIType(UI_TYPE eType) { m_eBeforeUIType = eType; }
	virtual void SetPlayer(CObj* pPlayer);
	void SetUIType(UI_TYPE eType) { m_eUIType = eType; }
	void SetHeadSize(float x, float y) { m_tHeadSize.x = x, m_tHeadSize.y = y; }
	void SetHeadSize(_SIZE tSize) { m_tHeadSize = tSize; }

public:
	UI_TYPE GetBeforeUIType() const { return m_eBeforeUIType; }
	UI_TYPE GetUIType() const { return m_eUIType; }
	_SIZE GetHeadSize() const { return m_tHeadSize; }
	
protected:
	CUI* FindUI(UI_TYPE eType);
	CUI* FindNextUI(UI_TYPE eType); // eType �������� ���� �־��� ui ã��(eType�� before�� ���� ��ü)

public:
	void ReleasePlayer();

public:
	virtual bool Init(UI_TYPE eType);
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUI* Clone() = 0;

public:
	void UseUIIcon();

public:
	void SetZOrder(int iZOrder);

public:
	void OpenUI();
	void CloseUI();

public:
	int GetZOrder() const { return m_iZOrder; }
//
public:
	CButton* FindButton(const string& strTag);
	CTexture* FindUITexture(const string& strTag);
	CIcon* FindIcon(const string& strTag); // tag ��ġ�� ������ ã��
	string FindIconKey(CIcon* pIcon); // �������� ���� key tag ã��
	bool FindIcon(CIcon* pIcon);
	CTexture* FindKeyImage(const string& strTag);

public:
	void AddIcon(const string& strTag, CIcon* pIcon);
	void EraseIcon(const string& strTag);

public:
	CButton* AddButton(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	CTexture* AddUITexture(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	CTab* AddTab(const string & strKey, const wchar_t * pFileName, POSITION& tPos,
		POSITION& tOrigin, POSITION& tLT, POSITION& tRB);
	CKeyIcon* AddKeyIcon(const string & strTag, const wchar_t * pFileName, const string& strKey);

public:
	void SetPosition(const string& strKey, POSITION& tPos);

public:
	POSITION FindPosition(const string& strKey);
};

