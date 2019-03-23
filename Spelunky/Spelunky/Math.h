#pragma once
/****************************************************************************
## Math ##
@@ Author : �Ǽ��� , Date : 2019.03.19
*****************************************************************************/
class Math final
{
public:
	static const float PI;					//PI 3.14f
	static const float Epsilon;				//���Ƿ�(�ε��Ҽ� ���� ����)
	static const int IntMin;				//int �ּ�
	static const int IntMax;				//int �ִ�
	static const float FloatMin;			//float �ּ�
	static const float FloatMax;			//float �ִ�

	static float NegativeChecking(const float& value);		//���� ��� �Ǵ�
	static float ToRadian(const float& degree);			//��׸� -> ����
	static float ToDegree(const float& radian);			//���� -> ��׸� 
		   
	static int Random(const int& r1, const int& r2);				//int ������ 
	static float Random(const float& r1, const float& r2);		//float ������
		   
	static float RandF();							//float ������
		   
	static float Lerp(const float& val1, const float& val2, const float& amount);	//���� ������
	static float Clampf(float value, const float& min, const float& max);		//value�� min,max������ �Ѿ���ϸ� min,max�� ��ȯ
	static int Clamp(int value, const int& min, const int& max);				//''
		   
	static int FloatToInt(const float& f);
	static bool FloatEqual(const float& f1, const float& f2);

	//�� �������� �Ÿ� ��ȯ
	static float GetDistance(const float& startX, const float& startY, const float& endX, const float& endY);
	//�� �� ������ ���� ��ȯ
	static float GetAngle(const float& x1, const float& y1, const float& x2, const float& y2);

	/*********************************************************
	## Min ##
	�� �� �߿��� ���� ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	**********************************************************/
	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}
	/**********************************************************
	## Max ##
	�� �� �߿��� ū ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	**********************************************************/
	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	/**********************************************************
	## Lerp ##
	a��b�� ���� ���� ���� ���� ��ȯ
	@@ T a : ��
	@@ T b : ��
	@@ float t : 0.0f ~ 1.0f ������ ��(a �� b ������ ���߰�(����))
	**********************************************************/
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b - a)*t;
	}
	/**********************************************************
	## Abs ##
	���밪 ��ȯ
	@@ T a : ��
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