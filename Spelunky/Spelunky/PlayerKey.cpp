#include "stdafx.h"
#include "PlayerKey.h"

#include "Player.h"
PlayerKey::PlayerKey()
{
	mPlayerKey[(int)PlayerKey::Key::Left] = 'A';
	mPlayerKey[(int)PlayerKey::Key::Right] = 'D';
	mPlayerKey[(int)PlayerKey::Key::Down] = 'S';
	mPlayerKey[(int)PlayerKey::Key::Jump] = 'W';
	mPlayerKey[(int)PlayerKey::Key::Bomb] = 'R';
	mPlayerKey[(int)PlayerKey::Key::Attack] = VK_LBUTTON;
	mPlayerKey[(int)PlayerKey::Key::Zoom] = VK_RBUTTON;
	mPlayerKey[(int)PlayerKey::Key::Shift] = VK_LSHIFT;
	mPlayerKey[(int)PlayerKey::Key::Interaction] = 'E';

	for (int i = 0; i < (int)Key::End; ++i)
		mPlayerKeyState[i] = PlayerKey::KeyState::None;
}


PlayerKey::~PlayerKey(){}

void PlayerKey::Update(RePlayDatas<InputData>* pInputDatas)
{
	/*********************************************************************
	## KeyStay ##
	**********************************************************************/
	this->CheckPreKeyState();

	/*********************************************************************
	## KeyDown
	**********************************************************************/
	bool isInput = false;
	for (int i = 0; i < (int)PlayerKey::Key::End; ++i)
	{
		if (_Input->GetKeyDown(mPlayerKey[i]))
		{
			mPlayerKeyState[i] = KeyState::Down;
			isInput = true;
		}
		else if (_Input->GetKeyUp(mPlayerKey[i]))
		{ 
			mPlayerKeyState[i] = KeyState::Up;
			isInput = true;
		}
	}
	//if (isInput)
	{
		if (pInputDatas)
		{
			InputData data;
			memcpy(data.keyState, mPlayerKeyState, sizeof(int) * 9);
			pInputDatas->UpdateInfo(data);
		}
	}

}

void PlayerKey::CheckPreKeyState()
{
	for (int i = 0; i < (int)Key::End; ++i)
	{
		if (mPlayerKeyState[i] == KeyState::Down)
			mPlayerKeyState[i] = KeyState::Stay;
		else if (mPlayerKeyState[i] == KeyState::Up)
			mPlayerKeyState[i] = KeyState::None;
	}
}

void PlayerKey::CopyKeyState(void * pKeyArr)
{
	memcpy(&mPlayerKeyState, pKeyArr, sizeof(int) * (int)PlayerKey::Key::End);
}

void PlayerKey::SetupKey(const PlayerKey::Key & key, const int & keyboard)
{
	mPlayerKey[(int)key] = keyboard;
}

const int & PlayerKey::GetPlayerKey(PlayerKey::Key key)
{
	return mPlayerKey[(int)key];
}

void PlayerKey::PushKey(const Key & key)
{
	mPlayerKeyState[(int)key] = KeyState::Down;
}

void PlayerKey::PopKey(const Key & key)
{
	mPlayerKeyState[(int)key] = KeyState::Up;
}
