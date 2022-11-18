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
	// 카메라 위치를 고려하여 위치 조정
	if (bCam == true)
		tTempPos -= GET_SINGLE(CCamera)->GetPos();
	
	// tTempPos 기준 texture 사각형 범위
	D2D1_RECT_F tRect;
	tRect.left = tTempPos.x + m_tLT.x;
	tRect.top = tTempPos.y + m_tLT.y;
	/*tRect.right = tTempPos.x + m_tRB.x;
	tRect.bottom = tTempPos.y + m_tRB.y;*/
	
	float tempWidth = (m_tRB.x - m_tLT.x) * fWidth;
	tRect.right = tRect.left + tempWidth;
	float tempHeight = (m_tRB.y - m_tLT.y) * fHeight;
	tRect.bottom = tRect.top + tempHeight;

	// 모든 texture는 왼쪽 기준이므로 오른쪽을 향하는 경우
	// 좌우반전 후 출력
	// 다시 좌우반전 되돌려주기
	if (eDir == MD_RIGHT)
	{
		// 좌표 기준 대칭을 위해 좌표 위치 보정
		tRect.left -= 2 * tTempPos.x;
		tRect.right -= 2 * tTempPos.x;

		// 좌우 대칭 행렬 정의
		D2D1_MATRIX_3X2_F mat;
		mat._11 = -1; mat._12 = 0;
		mat._21 = 0; mat._22 = 1;
		mat._31 = 0; mat._32 = 0;

		pRenderTarget->SetTransform(&mat);
		pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);

		// 다시 되돌리기
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	else //if (m_eDir == MD_LEFT)
	{
		// 인자로 받아오는 각도는 다음과 같다.
		//			270
		//
		//	180				0(기준)
		//
		//			90
		// 렌더링에 사용되는 각도 기준은 다음과 같다.
		//			0(기준)
		//
		//	-90					90
		//
		//			-180=180
		// 따라서 내부 변환 거쳐주기
		if(fAngle == 270.f)
			pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);
		else // 회전이 필요하다면
		{
			// 각도 변환
			if (0.f <= fAngle && fAngle < 90)
				fAngle += 90.f;
			else //if (90 <= fAngle && fAngle < 360)
				fAngle -= 270.f;
			// 물체 중심기준으로 회전하되, 윗방향 축을 기준으로
			// 양수면 시계방향, 음수면 반시계방향 회전, 각도는 라디안이 아니라 도
			// 중점 구하기
			D2D1_POINT_2F p;
			/*p.x = (tRect.right + tRect.left) / 2.f;
			p.y = (tRect.bottom + tRect.top) / 2.f;*/
			p.x = tTempPos.x;
			p.y = tTempPos.y;

			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(fAngle, p));
			pRenderTarget->DrawBitmap(m_pD2DBitmap, &tRect, opacity);
			// 다시 되돌려주자.
			pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		}
	}
}
