#pragma once
/****************************************************************************
## Math ##
@@ Author : 권순우 , Date : 2019.03.19
*****************************************************************************/
class Math final
{
private:
	static UINT _randomCount;
	static UINT _randomSid;
	static UINT64 _frameCount;
	static deque<pair<UINT64,UINT>> _randomList;
public:
	static void SetRandomSid(const UINT& sid) { _randomSid = sid; }
	static UINT GetRandomSid() { return _randomSid; }
	static void SetRandomCount(const UINT& count = 0) { _randomCount = count; }
	static void ResetAllRandomValue()
	{
		_randomSid = (UINT)time(NULL);
		srand(_randomSid);
		_randomCount = 0;
		_frameCount = 0;
		_randomList.clear();
	}
	static void UpdateRandomCount(const UINT64& frame)
	{
		if (frame % 100 == 0)
		{
			_randomList.push_back(make_pair(frame, _randomCount));
			_frameCount = 1;
			if (_randomList.size() >= 10)
				_randomList.pop_front();
		}
	}
	static void InitRandomSystem(const UINT64& frame)
	{
		for (UINT i = 0; i < _randomList.size(); ++i)
		{
			if (_randomList[i].first == frame)
			{
				srand(_randomSid);
				for (UINT j = 0; j < _randomList[i].second; ++j)
					rand();
				break;
			}
		}
	}

public:
	static const float PI;					//PI 3.14f
	static const float Epsilon;				//엡실론(부동소수 오차 범위)
	static const int IntMin;				//int 최소
	static const int IntMax;				//int 최대
	static const float FloatMin;			//float 최소
	static const float FloatMax;			//float 최대

	static float NegativeChecking(const float& value);		//음수 양수 판단
	static float ToRadian(const float& degree);			//디그리 -> 라디안
	static float ToDegree(const float& radian);			//라디안 -> 디그리 
		   
	static int Random(const int& r1, const int& r2);				//int 랜덤값 
	static float Random(const float& r1, const float& r2);		//float 랜덤값
	static float RandF();							//float 랜덤값

	static float RandNegative();					//랜덤 -1,1값 
	static bool RandomBool();
	static bool PercentageBool(const float& percentage);
		   
	static float Lerp(const float& val1, const float& val2, const float& amount);	//선형 보간법
	static float Clampf(float value, const float& min, const float& max);		//value가 min,max범위를 넘어설려하면 min,max를 반환
	static int Clamp(int value, const int& min, const int& max);				//''
		   
	static int FloatToInt(const float& f);
	static bool FloatEqual(const float& f1, const float& f2);

	static float FixFloat(float f);
	//두 점사이의 거리 반환
	static float GetDistance(const float& startX, const float& startY, const float& endX, const float& endY);
	//두 점 사이의 각을 반환
	static float GetAngle(const float& x1, const float& y1, const float& x2, const float& y2);


	/*********************************************************
	## Min ##
	두 수 중에서 작은 수를 반환
	@@ T a : 수
	@@ T b : 수
	**********************************************************/
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	/**********************************************************
	## Max ##
	두 수 중에서 큰 수를 반환
	@@ T a : 수
	@@ T b : 수
	**********************************************************/
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	/**********************************************************
	## Lerp ##
	a와b에 대한 선형 보간 값을 반환
	@@ T a : 수
	@@ T b : 수
	@@ float t : 0.0f ~ 1.0f 사이의 값(a 와 b 사이의 가중값(비율))
	**********************************************************/
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}
	/**********************************************************
	## Abs ##
	절대값 반환
	@@ T a : 수
	**********************************************************/
	template<typename T>
	static T Abs(const T& a)
	{
		if (a >= 0.0f)
			return a;
		else
			return -a;
	}

};