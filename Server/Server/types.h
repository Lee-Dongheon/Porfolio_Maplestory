#pragma once

typedef struct _tagResolution // 해상도 구조체
{
	int iW;
	int iH;

	_tagResolution() :
		iW(0),
		iH(0)
	{
	}

	_tagResolution(int x, int y) :
		iW(x),
		iH(y)
	{
	}

}RESOLUTION, *PRESOLUTION;

typedef struct _tagPosition // float 좌표 구조체
{
	float x, y;

	_tagPosition() :
		x(0.f),
		y(0.f)
	{
	}

	_tagPosition(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}
	_tagPosition(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	_tagPosition(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator=(const _tagPosition& pos)
	{
		x = pos.x;
		y = pos.y;
	}

	void operator=(const POINT& pt)
	{
		x = (float)pt.x;
		y = (float)pt.y;
	}

	void operator=(float f[2])
	{
		x = f[0];
		y = f[1];
	}

	//========================== + 연산
	_tagPosition operator+(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x + pos.x;
		tPos.y = y + pos.y;

		return tPos;
	}

	_tagPosition operator+(const POINT& ps) const
	{
		_tagPosition tPos;
		tPos.x = x + ps.x;
		tPos.y = y + ps.y;

		return tPos;
	}

	_tagPosition operator+(float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x + f[0];
		tPos.y = y + f[1];

		return tPos;
	}

	_tagPosition operator+(float f) const
	{
		_tagPosition tPos;
		tPos.x = x + f;
		tPos.y = y + f;

		return tPos;
	}
	void operator+=(const _tagPosition& pos)
	{
		x += pos.x;
		y += pos.y;
	}

	//========================== - 연산
	_tagPosition operator-(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x - pos.x;
		tPos.y = y - pos.y;

		return tPos;
	}

	_tagPosition operator-(const POINT& ps) const
	{
		_tagPosition tPos;
		tPos.x = x - ps.x;
		tPos.y = y - ps.y;

		return tPos;
	}

	_tagPosition operator-(float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x - f[0];
		tPos.y = y - f[1];

		return tPos;
	}

	_tagPosition operator-(float f) const
	{
		_tagPosition tPos;
		tPos.x = x - f;
		tPos.y = y - f;

		return tPos;
	}

	void operator-=(const _tagPosition& pos)
	{
		x -= pos.x;
		y -= pos.y;
	}

	//========================== * 연산
	_tagPosition operator*(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x * pos.x;
		tPos.y = y * pos.y;

		return tPos;
	}

	_tagPosition operator*(const POINT& ps) const
	{
		_tagPosition tPos;
		tPos.x = x * ps.x;
		tPos.y = y * ps.y;

		return tPos;
	}

	_tagPosition operator*(float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x * f[0];
		tPos.y = y * f[1];

		return tPos;
	}

	_tagPosition operator*(float f) const
	{
		_tagPosition tPos;
		tPos.x = x * f;
		tPos.y = y * f;

		return tPos;
	}
	//========================== / 연산
	_tagPosition operator/(const _tagPosition& pos) const
	{
		_tagPosition tPos;
		tPos.x = x / pos.x;
		tPos.y = y / pos.y;

		return tPos;
	}

	_tagPosition operator/(const POINT& ps) const
	{
		_tagPosition tPos;
		tPos.x = x / ps.x;
		tPos.y = y / ps.y;

		return tPos;
	}

	_tagPosition operator/(float f[2]) const
	{
		_tagPosition tPos;
		tPos.x = x / f[0];
		tPos.y = y / f[1];

		return tPos;
	}

	_tagPosition operator/(float f) const
	{
		_tagPosition tPos;
		tPos.x = x / f;
		tPos.y = y / f;

		return tPos;
	}

	// ui 좌표 순서를 위한 비교연산 오버로딩
	bool operator>(const _tagPosition& pos) const
	{
		if (y > pos.y)
			return true;
		else if (y < pos.y)
			return false;
		else // y == pos.y
		{
			return x > pos.x;
		}
	}
	bool operator<(const _tagPosition& pos) const
	{
		if (y < pos.y)
			return true;
		else if (y > pos.y)
			return false;
		else // y == pos.y
		{
			return x < pos.x;
		}
	}
	bool operator==(const _tagPosition& pos) const
	{
		return (x == pos.x && y == pos.y);
	}
	bool operator>=(const _tagPosition& pos) const
	{
		return operator>(pos) || operator==(pos);
	}
	bool operator<=(const _tagPosition& pos) const
	{
		return operator<(pos) || operator==(pos);
	}
	// ui 좌표 순서를 위한 순서함수
	void Next(int xMax)
	{
		x += 1.f;
		if (x >= xMax)
		{
			x = 0.f;
			y += 1.f;
		}
	}
}POSITION, *PPOSITION, _SIZE, *_PSIZE;

typedef struct _tagRectangle
{
	float l;
	float t;
	float r;
	float b;

	_tagRectangle() :
		l(0.f),
		t(0.f),
		r(0.f),
		b(0.f)
	{
	}

	_tagRectangle(float fL, float fT, float fR, float fB) :
		l(fL),
		t(fT),
		r(fR),
		b(fB)
	{
	}

	void operator+=(POSITION tPos)
	{
		l += tPos.x;
		t += tPos.y;
		r += tPos.x;
		b += tPos.y;
	}

	void operator-=(POSITION tPos)
	{
		l -= tPos.x;
		t -= tPos.y;
		r -= tPos.x;
		b -= tPos.y;
	}

}RECTANGLE, *PRECTANGLE;

typedef struct _tagSphere
{
	POSITION tCenter;
	float fRadius;

	_tagSphere() :
		tCenter(0.f, 0.f),
		fRadius(0.f)
	{
	}

	_tagSphere(POSITION tPos, float r) :
		tCenter(tPos),
		fRadius(r)
	{

	}
}SPHERE, *PSPHERE;

typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	_tagPixel(unsigned char x, unsigned char y, unsigned char z) :
		r(x), g(y), b(z)
	{

	}
	_tagPixel() :
		r(0), g(0), b(0)
	{

	}
}PIXEL, *PPIXEL;