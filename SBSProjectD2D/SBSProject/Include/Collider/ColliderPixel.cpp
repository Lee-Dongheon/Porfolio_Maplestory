#include "ColliderPixel.h"
#include "../Object/Obj.h"
#include "ColliderRect.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Texture.h"

CColliderPixel::CColliderPixel()/*:
	m_pTexture(NULL)*/
{
	m_eCollType = CT_PIXEL;
	m_iWidth = 0;
	m_iHeight = 0;
	m_HitColor = PIXEL(255, 0, 255);
}

CColliderPixel::CColliderPixel(const CColliderPixel & coll) :
	CCollider(coll)
{
	m_iWidth = coll.m_iWidth;
	m_iHeight = coll.m_iHeight;
}


CColliderPixel::~CColliderPixel()
{
	//SAFE_RELEASE(m_pTexture);
}

bool CColliderPixel::SetPixelInfo(const char* pFileName)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pFileName , "rb");

	if (!pFile)
		return false;

	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;

	fread(&fh, sizeof(fh), 1, pFile);
	fread(&ih, sizeof(ih), 1, pFile);

	m_iWidth = ih.biWidth;
	m_iHeight = ih.biHeight;

	m_vecPixel.clear();

	m_vecPixel.resize(m_iWidth * m_iHeight);

	// �ٹٲ� ���ڸ� �����ϰ� �� �پ� �������� �����Ѵ�.
	for (int i = m_iHeight; i > 0; i--)
	{
		fread(&m_vecPixel[(i - 1) * m_iWidth], sizeof(PIXEL), m_iWidth, pFile);
		// �� �� bgr ������ �޾ƿ��Ƿ� ���� �� �ٲ��ֱ�
		// debug �ÿ� ������� �ʰڴ�(colorkey�� �ݴ�� �Է����ָ� ��)
#ifndef _DEBUG
		for (int j = 0; j < m_iWidth; j++)
		{
			unsigned char temp = m_vecPixel[(i - 1) * m_iWidth + j].r; // ���� b��
			m_vecPixel[(i - 1) * m_iWidth + j].r = m_vecPixel[(i - 1) * m_iWidth + j].b; // ���� r��
			m_vecPixel[(i - 1) * m_iWidth + j].b = temp;
		}
#endif
		// ���� ��Ʈ �о��ֱ�
		char temp;
		fread(&temp, sizeof(char), 1, pFile);
		// ¦�� ���̸� �ѹ���
		if(m_iWidth % 2 == 0)
			fread(&temp, sizeof(char), 1, pFile);
	}

	// ���� ��Ʈ�� ���� ����
	// ���� ���̰� Ȧ���� ��� ���� ��Ʈ�� ���� �� 1
	// �� 1 * height
	// ¦�� �� ��� ���� ��Ʈ�� ���� �� 2, �� 2 * height

	fclose(pFile);

	return true;
}
bool CColliderPixel::MakePixelVec(int iWidth, int iHeight, PIXEL& p)
{
	// Height 1�� �����Ұ����� �ڽ��� ������� ������ ��������
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			m_vecPixel.push_back(p);
		}
	}

	return true;
}
//
//void CColliderPixel::SetTexture(const string & strKey, const wchar_t * pFileName, POSITION & tOrigin, POSITION & tLT, POSITION & tRB)
//{
//	SAFE_RELEASE(m_pTexture);
//	_SIZE tSize = tRB - tLT;
//	m_pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, tOrigin, tSize, tLT, tRB);
//}

bool CColliderPixel::Init()
{
	return true;
}

void CColliderPixel::Input(float fDeltaTime)
{
}

int CColliderPixel::Update(float fDeltaTime)
{
	return 0;
}

int CColliderPixel::LateUpdate(float fDeltaTime)
{
	return 0;
}

bool CColliderPixel::Collision(CCollider * pDest)
{
	switch (pDest->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToPixel(((CColliderRect*)pDest)->GetWorldInfo(), m_tPos, m_vecPixel, m_iWidth, m_iHeight, m_HitColor);
	}
	return false;
}

void CColliderPixel::Render(ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime)
{
	CCollider::Render(pGDIRenderTarget, pRenderTarget, fDeltaTime);

	// ī�޶� ��ġ�� ����Ͽ� ��ġ ����
	POSITION tPos = m_tPos - GET_SINGLE(CCamera)->GetPos();

}

CColliderPixel * CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}
