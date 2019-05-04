#include "stdafx.h"
#include "Math.h"

UINT Math::_randomCount = 0;
UINT Math::_randomSid = 0;
UINT64 Math::_frameCount = 0;
deque<pair<UINT64, UINT>> Math::_randomList;


const float Math::PI = 3.14159265f;
const float Math::Epsilon = 1E-6f;
const int Math::IntMin = -2147483647;
const int Math::IntMax = 2147483647;
const float Math::FloatMin = -3.402823E+38f;
const float Math::FloatMax = 3.402823E+38f;
/**********************************************************
## NegativeChecking ##
소수의 음수 양수를 판단해 ~1.f또는 1.f 반환
@@ float value : 값
**********************************************************/
 float Math::NegativeChecking(const float& value)
{
	if (value < 0.0f)
		return -1.0f;
	else
		return 1.0f;
}

/**********************************************************
## ToRadian ##
디그리값을 라디안으로 변환해 반환
@@ float degree : 디그리값
**********************************************************/
 float Math::ToRadian(const float& degree)
{
	return degree * Math::PI / 180.0f;
}
/**********************************************************
## ToDegree ##
라디안을 디그리로 변환해 반환
@@ float radian : 라디안 값
**********************************************************/
 float Math::ToDegree(const float& radian)
{
	return radian * 180.0f / Math::PI;
}
/**********************************************************
## Random ##
@@ float r1 : 시작 값
@@ float r2 : 끝 값
**********************************************************/
 int Math::Random(const int & r1, const int & r2)
{
	 ++_randomCount;
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}
/**********************************************************
## Random ##
@@ float r1 : 시작 값
@@ float r2 : 끝 값
**********************************************************/
 float Math::Random(const float& r1, const float& r2)
{
	 ++_randomCount;
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}
/**********************************************************
## RandF ##
소수형 난수 반환
**********************************************************/
 float Math::RandF()
{
	 ++_randomCount;
	return (float)(rand()) / (float)RAND_MAX;
}
float Math::RandNegative()
{
	int randomDot = Math::Random(0, 1);
	float result = -1.f;
	for (int i = 0; i < randomDot; ++i)
		result *= -1.f;
	return result;
}
bool Math::RandomBool()
{
	return static_cast<bool>(Math::Random(0, 1));
}
bool Math::PercentageBool(const float & percentage)
{
	if (Math::Random(0.f, 1.f) < percentage)
		return true;
	return false;
}
/**********************************************************
## Lerp ##
비율을 통해 두 값을 선형 보간해서 반환
@@ float val1 : 시작 값
@@ float val2 : 도달 값
@@ float amount : 두 값의 비율, 0.0f ~ 1.0f 사이의 값이 들어간다.
**********************************************************/
 float Math::Lerp(const float& val1, const float& val2, const float& amount)
{
	return val1 + (val2 - val1) * amount;
}
/**********************************************************
## Clampf ##
어떠한 값이 min,max보다 크거나 작아지려고 하면 min또는max를 반환
@@ float value : 값
@@ float min : 최소 범위
@@ float max : 최대 범위
**********************************************************/
 float Math::Clampf(float value, const float& min, const float& max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}
/**********************************************************
## Clamp ##
어떠한 값이 min,max보다 크거나 작아지려고 하면 min또는max를 반환
@@ int value : 값
@@ int min : 최소 범위
@@ int max : 최대 범위
**********************************************************/
 int Math::Clamp(int value, const int& min, const int& max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}


/**********************************************************
## FloatToInt ##
@@ const float& f : int로 형변환할 float
**********************************************************/
 int Math::FloatToInt(const float & f)
{
	return static_cast<int>(f + Math::Epsilon);
}
/**********************************************************
## FloatEqual ##
두 float이 부동소수점 오차 내에 있는지 비교

@@ const float& f1 : float 1
@@ const float& f2 : float 2
**********************************************************/
 bool Math::FloatEqual(const float & f1, const float & f2)
{
	return (fabs(f1 - f2) <= Math::Epsilon);
}

float Math::FixFloat(float f)
{
	return ((int)(f * pow(10.0f, 3))) / pow(10.0f, 3);
}

/*********************************************************************
## GetDistance ##
두 점 사이의 거리를 구한다
@@ float startX : 시작점 X
@@ float startY : 시작점 y
@@ float endX : 목표점 x
@@ float endY : 목표점 y
**********************************************************************/
 float Math::GetDistance(const float& startX, const float& startY, const float& endX, const float& endY)
{
	float x = endX - startX;
	float y = endY - startY;

	return sqrt(x * x + y * y);
}
/*******************************************************************************
## GetAngle ##
두 점 사이의 각을 구한다.
@@ float x1 : 시작점 X
@@ float y1 : 시작점 Y
@@ float x2 : 목표점 X
@@ float y2 : 목표점 Y
*******************************************************************************/
 float Math::GetAngle(const float& x1, const float& y1, const float& x2, const float& y2)
{
	//x축과 y축으로의 길이를 구한다. 
	float x = x2 - x1;
	float y = y2 - y1;
	//피타고라스 정리로 대각선의 길이 구함
	float distance = sqrt((x * x) + (y * y));
	//삼각함수의 원리에 의해 cos(angle) = x / 빗변  == angle = acos(x / 빗변);
	float angle = acos(x / distance);
	//만약 구하려는 각이 뒤집어져 있는 상황이라면 
	if (y2 > y1)
	{
		//역으로 360에서 -한값을 구한다. 
		angle = Math::PI * 2.0f - angle;
		if (angle >= Math::PI * 2.0f)
			angle -= Math::PI * 2.0f;
	}

	return angle;
}

float Math::Floor(const float & f, const int & cipher)
{
	int tempCipher = 10;
	for (int i = 0; i < cipher;++i)
		tempCipher *= 10;
	float result = std::floorf(((f + Math::Epsilon) * (float)tempCipher)) / (float)tempCipher;
	return result;
}

