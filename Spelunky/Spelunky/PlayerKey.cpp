#include "stdafx.h"
#include "PlayerKey.h"


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

void PlayerKey::Update()
{
	/*********************************************************************
	## KeyStay ##
	**********************************************************************/
	this->CheckPreKeyState();

	/*********************************************************************
	## KeyDown
	**********************************************************************/
	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Left]))
		mPlayerKeyState[(int)PlayerKey::Key::Left] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Right]))
		mPlayerKeyState[(int)PlayerKey::Key::Right] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Down]))
		mPlayerKeyState[(int)PlayerKey::Key::Down] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Jump]))
		mPlayerKeyState[(int)PlayerKey::Key::Jump] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Bomb]))
		mPlayerKeyState[(int)PlayerKey::Key::Bomb] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Attack]))
		mPlayerKeyState[(int)PlayerKey::Key::Attack] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Shift]))
		mPlayerKeyState[(int)PlayerKey::Key::Shift] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Interaction]))
		mPlayerKeyState[(int)PlayerKey::Key::Interaction] = KeyState::Down;

	if (_Input->GetKeyDown(mPlayerKey[(int)PlayerKey::Key::Zoom]))
		mPlayerKeyState[(int)PlayerKey::Key::Zoom] = KeyState::Down;

	/*********************************************************************
	## KeyUp ##
	**********************************************************************/
	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Left]))
		mPlayerKeyState[(int)PlayerKey::Key::Left] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Right]))
		mPlayerKeyState[(int)PlayerKey::Key::Right] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Down]))
		mPlayerKeyState[(int)PlayerKey::Key::Down] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Jump]))
		mPlayerKeyState[(int)PlayerKey::Key::Jump] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Bomb]))
		mPlayerKeyState[(int)PlayerKey::Key::Bomb] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Attack]))
		mPlayerKeyState[(int)PlayerKey::Key::Attack] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Shift]))
		mPlayerKeyState[(int)PlayerKey::Key::Shift] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Interaction]))
		mPlayerKeyState[(int)PlayerKey::Key::Interaction] = KeyState::Up;

	if (_Input->GetKeyUp(mPlayerKey[(int)PlayerKey::Key::Zoom]))
		mPlayerKeyState[(int)PlayerKey::Key::Zoom] = KeyState::Up;
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
