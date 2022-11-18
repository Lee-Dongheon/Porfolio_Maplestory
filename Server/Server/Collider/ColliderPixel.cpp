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
		if (m_iWidth % 2 == 0)
			fread(&temp, sizeof(char), 1, pFile);
	}

	// ���� ��Ʈ�� ���� ����
	// ���� ���̰� Ȧ���� ��� ���� ��Ʈ�� ���� �� 1
	// �� 1 * height
	// ¦�� �� ��� ���� ��Ʈ�� ���� �� 2, �� 2 * height

	//fread(&m_vecPixel[0], sizeof(PIXEL), m_vecPixel.size(), pFile);
	//

	//// �� �Ʒ��� ���������ش�.
	//for (int i = 0; i < m_iHeight / 2; ++i)
	//{
	//	// ���� �ε����� �ȼ� �� ���� �����صд�.
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
