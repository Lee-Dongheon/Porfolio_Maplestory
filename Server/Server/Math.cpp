#include "Math.h"
//#include <math.h>

float GetDistance(const POSITION & tPos1, const POSITION & tPos2)
{
	float x, y;
	x = tPos2.x - tPos1.x;
	y = tPos2.y - tPos1.y;

	return sqrt(x * x + y * y);
}

float GetDistance(const POINT & tPos1, const POSITION & tPos2)
{
	float x, y;
	x = tPos2.x - (float)tPos1.x;
	y = tPos2.y - (float)tPos1.y;

	return sqrt(x * x + y * y);
}

bool PointInRect(const RECTANGLE & Rect, const POSITION & tPos)
{
	if (tPos.x > Rect.r)
		return false;
	else if (tPos.x < Rect.l)
		return false;
	else if (tPos.y > Rect.b)
		return false;
	else if (tPos.y < Rect.t)
		return false;

	return true;
}

float ChangeAngle(const POSITION & tTargetPos, const POSITION & tPos, float & fAngle, float fDeltaTime)
{
	POSITION tGap = tTargetPos - tPos;
	float fTargetAngle = 0.f;

	if (tGap.x == 0.f)
	{
		if (tGap.y > 0.f) // �Ʒ�����
			fTargetAngle = 90.f;
		else if (tGap.y < 0.f) // ������
			fTargetAngle = 270.f;
		else // �� ���� �ƿ� ��ģ ��, �����ϸ� �ȵǴ� ���
			return 0.f;
	}
	else if (tGap.x < 0.f)
	{
		fTargetAngle = 180.f + atan(tGap.y / tGap.x) * 180.f / PI;
	}
	else //if(tGap.x > 0.f)
		fTargetAngle = atan(tGap.y / tGap.x) * 180.f / PI;

	float fTempAngle = fTargetAngle - fAngle;
	// �� ª�������� ȸ���ϵ���
	if (fTempAngle > 180.f)
		fTempAngle = 360.f - fTempAngle;
	if (fTempAngle < -180.f)
		fTempAngle = 360.f + fTempAngle;

	if (fTempAngle < 0.f)
		fAngle += (-360.f * fDeltaTime < fTempAngle ? fTempAngle : -360.f * fDeltaTime);
	else
		fAngle += (360.f * fDeltaTime > fTempAngle ? fTempAngle : 360.f * fDeltaTime);

	return fAngle;
}
