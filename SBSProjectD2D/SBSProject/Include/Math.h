#pragma once

#include "Game.h"

float GetDistance(const POSITION& tPos1, const POSITION& tPos2);
float GetDistance(const POINT& tPos1, const POSITION& tPos2);

bool PointInRect(const RECTANGLE& Rect, const POSITION& tPos);
float ChangeAngle(const POSITION& tTargetPos, const POSITION& tPos, float& fAngle, float fDeltaTime);

bool RectToRect(const RECTANGLE& src, const RECTANGLE& dest);