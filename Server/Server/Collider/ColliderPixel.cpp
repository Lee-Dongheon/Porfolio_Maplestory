#include "ColliderPixel.h"
#include "ColliderRect.h"

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
}

bool CColliderPixel::SetPixelInfo(const char* pFileName)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, pFileName, "rb");

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

	// 줄바꿈 문자를 제외하고 한 줄씩 역순으로 저장한다.
	for (int i = m_iHeight; i > 0; i--)
	{
		fread(&m_vecPixel[(i - 1) * m_iWidth], sizeof(PIXEL), m_iWidth, pFile);
		// 이 때 bgr 순으로 받아오므로 저장 후 바꿔주기
		// debug 시엔 사용하지 않겠다(colorkey를 반대로 입력해주면 됨)
#ifndef _DEBUG
		for (int j = 0; j < m_iWidth; j++)
		{
			unsigned char temp = m_vecPixel[(i - 1) * m_iWidth + j].r; // 실제 b값
			m_vecPixel[(i - 1) * m_iWidth + j].r = m_vecPixel[(i - 1) * m_iWidth + j].b; // 실제 r값
			m_vecPixel[(i - 1) * m_iWidth + j].b = temp;
		}
#endif
		// 남는 비트 읽어주기
		char temp;
		fread(&temp, sizeof(char), 1, pFile);
		// 짝수 넓이면 한번더
		if (m_iWidth % 2 == 0)
			fread(&temp, sizeof(char), 1, pFile);
	}

	// 남는 비트에 대한 정보
	// 가로 넓이가 홀수인 경우 남는 비트가 가로 당 1
	// 총 1 * height
	// 짝수 인 경우 남는 비트가 가로 당 2, 총 2 * height

	//fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);
	//

	//// 위 아래를 반전시켜준다.
	//for (int i = 0; i < m_iHeight / 2; ++i)
	//{
	//	// 현재 인덱스의 픽셀 한 줄을 저장해둔다.
	//	memcpy(pPixelArr, &m_vecPixel[i * m_iWidth], sizeof(PIXEL) * m_iWidth);
	//	memcpy(&m_vecPixel[i * m_iWidth], &m_vecPixel[(m_iHeight - i - 1)*m_iWidth], sizeof(PIXEL) * m_iWidth);
	//	memcpy(&m_vecPixel[(m_iHeight - i - 1)*m_iWidth], pPixelArr, sizeof(PIXEL) * m_iWidth);
	//}

	//delete[] pPixelArr;

	fclose(pFile);

	return true;
}
bool CColliderPixel::MakePixelVec(int iWidth, int iHeight, PIXEL& p)
{
	// Height 1로 고정할거지만 박스를 만들수도 있으니 정석으로
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

CColliderPixel * CColliderPixel::Clone()
{
	return new CColliderPixel(*this);
}
