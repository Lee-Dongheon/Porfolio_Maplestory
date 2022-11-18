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
	static UI_TYPE m_eLastUIType; // 모든 UI 중에 마지막으로 활성화된 UI
	static CIcon* m_pSelectedIcon; // 마우스에 선택된 아이콘
	static bool m_bHaveSelectedIcon;
	static map<string, CTexture*> m_mapKeyImage; // 키 정보창 글자 이미지
	static map<string, CIcon*> m_mapIcon; // 키 이름과 아이콘 맵

protected:
	CPlayer* m_pPlayer;
	UI_TYPE m_eUIType;

	list<CTexture*> m_UITextureList; // UI에 들어가는 texture
	list<CButton*> m_ButtonList;
	unordered_map<string, CTab*> m_mapTab;
	map<string, POSITION> m_mapPos; // texture와 button들의 위치를 가지는 맵

	string m_strSelectedTab;
	int m_iZOrder;
	bool m_bIsSelected;
	POSITION m_tHeadSize; // 창의 머리부분 사이즈

	string m_strLastTab; // 활성화된 tab
	string m_strBeforeTab; // 직전까지 활성화된 tab
	bool m_bTabChanged; // tab 변화

	UI_TYPE m_eBeforeUIType; // 자신 직전에 활성화된 ui
	
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
	CUI* FindNextUI(UI_TYPE eType); // eType 다음으로 켜져 있었던 ui 찾기(eType을 before로 가진 객체)

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
	CIcon* FindIcon(const string& strTag); // tag 위치의 아이콘 찾기
	string FindIconKey(CIcon* pIcon); // 아이콘을 가진 key tag 찾기
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

