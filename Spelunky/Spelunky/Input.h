#pragma once
#include <bitset>
#include "SingletonBase.h"
/****************************************************************************
## Input ##
@@ Author : �Ǽ��� , Date : 2019.03.20
*****************************************************************************/
class Input final : public SingletonBase<Input> 
{
	BlockAssign(Input)
private:
	enum : int {KeyMax = 256};
private:
	//�ߺ� Ű ����� ���� ���� Ű�� ���� Ű ���� ���
	std::bitset<KeyMax> mKeyPast;
	std::bitset<KeyMax> mKeyCurrent;
	Vector2 mMousePosition;
private:
	friend class SingletonBase<Input>;
	Input();
	virtual ~Input();
public:
	void Update();
	//�ѹ� ���ȴٸ�
	bool GetKeyDown(int key);
	//Ű���� ���� ���ٸ�
	bool GetKeyUp(int key);
	//Ű�� ������ �ִٸ�
	bool GetKey(int key);
	//��� Ű
	bool ToggleKey(int key);

	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	inline Vector2 GetMousePosition()const { return this->mMousePosition; }
	POINT GetMousePositionPoint();
};

#define _Input Input::Get()
