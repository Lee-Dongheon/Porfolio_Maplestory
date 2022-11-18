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
	
	float m_fNSLength; // 미니맵 요소 N, S의 길이
	float m_fWELength; // 미니맵 요소 W, E의 길이
	POSITION m_tPlayerMinimapPos; // 미니맵상 플레이어 위치

	bool m_bMinimapChanged;

	map<string, CTexture*> m_mapMinimapTexture[MAP_END];
	// 순서는 min, small, max

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

