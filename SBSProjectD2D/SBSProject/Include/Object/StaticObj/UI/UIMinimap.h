#pragma once
#include "UI.h"

enum MAP_STATE
{
	MAP_DEFAULT = -1,
	MAP_MIN = 0,
	MAP_SMALL,
	MAP_MAX,
	MAP_END
};

class CUIMinimap :
	public CUI
{
public:
	CUIMinimap();
	CUIMinimap(const CUIMinimap &minimap);
	~CUIMinimap();

private:
	CTexture* m_pMinimapTexture;
	MAP_STATE m_eState;
	MAP_STATE m_eBeforeState;
	_SIZE m_tMapSize;
	_SIZE m_tMinimapSize;
	
	float m_fNSLength; // �̴ϸ� ��� N, S�� ����
	float m_fWELength; // �̴ϸ� ��� W, E�� ����
	POSITION m_tPlayerMinimapPos; // �̴ϸʻ� �÷��̾� ��ġ

	bool m_bMinimapChanged;

	map<string, CTexture*> m_mapMinimapTexture[MAP_END];
	// ������ min, small, max

public:
	void SetPlayer(CObj* pPlayer);
	void SetMinimapTexture(CTexture* pMinimap);
	void SetMinimapSize(_SIZE tSize) { m_tMinimapSize = tSize; }
	void SetMinimapSize(float x, float y) { m_tMinimapSize.x = x; m_tMinimapSize.y = y; }
	void SetMapSize(_SIZE tSize) { m_tMapSize = tSize; }
	void SetMapSize(float x, float y) { m_tMapSize.x = x; m_tMapSize.y = y; }

	void UseMinimapIcon();
	void UseIncrease();
	void UseDecrease();

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime);
	
	virtual CUIMinimap* Clone();
};

