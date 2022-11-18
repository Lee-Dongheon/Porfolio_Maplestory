#pragma once
#include "Collider.h"

class CColliderPixel :
	public CCollider
{
protected:
	friend class CObj;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& coll);
	~CColliderPixel();

private:
	vector<PIXEL> m_vecPixel;
	PIXEL m_HitColor;
	int m_iWidth;
	int m_iHeight;
	POSITION m_tPos;

public:
	void SetPos(float x, float y) { m_tPos.x = x; m_tPos.y = y; }
	void SetPos(POSITION& tPos) { m_tPos = tPos; }
	void SetHitColor(PIXEL p) { m_HitColor = p; }
	void SetHitColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_HitColor.r = r;
		m_HitColor.g = g;
		m_HitColor.b = b;
	}

public:
	const vector<PIXEL>& GetPixel() const { return m_vecPixel; }
	int GetWidth() const { return m_iWidth; }
	int GetHeight() const { return m_iHeight; }
	POSITION GetPos() const { return m_tPos; }
	PIXEL GetHitColor() const { return m_HitColor; }

public:
	bool SetPixelInfo(const char* pFileName);
	// 한줄짜리 픽셀 파일 만들바에 함수를 만든다.
	bool MakePixelVec(int iWidth, int iHeight, PIXEL& p);

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual bool Collision(CCollider* pDest);
	virtual CColliderPixel* Clone();

};

