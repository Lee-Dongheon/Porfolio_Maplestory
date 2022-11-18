#include "Texture.h"
#include "../Core/Camera.h"

CTexture::CTexture() :
	m_pD2DBitmap(NULL),
	m_tSize(_SIZE(0.f, 0.f)),
	m_tOrigin(POSITION(0.f, 0.f)),
	m_tLT(POSITION(0.f, 0.f)),
	m_tRB(POSITION(0.f, 0.f))
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pD2DBitmap);
}

void CTexture::Render(POSITION& tPos, ID2D1GdiInteropRenderTarget* pGDIRenderTarget, ID2D1HwndRenderTarget* pRenderTarget, float fDeltaTime, float opacity, bool bCam, MOVE_DIR eDir, float fAngle, float fWidth, float fHeight)
{
	POSITION tTempPos = tPos;
	// ī�޶� ��ġ�� ����Ͽ� ��ġ ����
	if (bCam == true)
		tTempPos -= GET_SINGLE(CCamera)->GetPos();
	
	// tTempPos ���� texture �簢�� ����
	D2D1_RECT_F tRect;
	tRect.left = tTempPos.x + m_tLT.x;
	tRect.top = tTempPos.y + m_tLT.y;
	/*tRect.right = tTempPos.x + m_tRB.x;
	tRect.bottom = tTempPos.y + m_tRB.y;*/
	
	float tempWidth = (m_tRB.x - m_tLT.x) * fWidth;
	tRect.right = tRect.left + tempWidth;
	float tempHeight = (m_tRB.y - m_tLT.y) * fHeight;
	tRect.bottom = tRect.top + tempHeight;

	// ��� texture�� ���� �����̹Ƿ� �������� ���ϴ� ���
	// �¿���� �� ���
	// �ٽ� �¿���� �ǵ����ֱ�
	if (eDir == MD_RIGHT)
	{
		// ��ǥ ���� ��Ī�� ���� ��ǥ ��ġ ����
		tRect.left -= 2 * tTempPos.x;
		tRect.right -= 2 * tTempPos.x;

		// �¿� ��Ī ��� ����
		D2D1_MATRIX_3X2_F mat;
		mat._11 = -1; mat._12 = 0;
		mat._21 = 0; mat._22 = 1;
		mat._31 = 0; mat._32 = 0;

		pRenderTarget->SetTransform(&mat);
		pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);

		// �ٽ� �ǵ�����
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	else //if (m_eDir == MD_LEFT)
	{
		// ���ڷ� �޾ƿ��� ������ ������ ����.
		//			270
		//
		//	180				0(����)
		//
		//			90
		// �������� ���Ǵ� ���� ������ ������ ����.
		//			0(����)
		//
		//	-90					90
		//
		//			-180=180
		// ���� ���� ��ȯ �����ֱ�
		if(fAngle == 270.f)
			pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);
		else // ȸ���� �ʿ��ϴٸ�
		{
			// ���� ��ȯ
			if (0.f <= fAngle && fAngle < 90)
				fAngle += 90.f;
			else //if (90 <= fAngle && fAngle < 360)
				fAngle -= 270.f;
			// ��ü �߽ɱ������� ȸ���ϵ�, ������ ���� ��������
			// ����� �ð����, ������ �ݽð���� ȸ��, ������ ������ �ƴ϶� ��
			// ���� ���ϱ�
			D2D1_POINT_2F p;
			/*p.x = (tRect.right + tRect.left) / 2.f;
			p.y = (tRect.bottom + tRect.top) / 2.f;*/
			p.x = tTempPos.x;
			p.y = tTempPos.y;

			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(fAngle, p));
			pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);
			// �ٽ� �ǵ�������.
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		}
	}
}
