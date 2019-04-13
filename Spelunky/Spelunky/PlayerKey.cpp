#include "stdafx.h"
#include "PlayerKey.h"


PlayerKey::PlayerKey()
{
	mPlayerKey[(int)PlayerKey::Key::Left] = 'A';
	mPlayerKey[(int)PlayerKey::Key::Right] = 'D';
	mPlayerKey[(int)PlayerKey::Key::Down] = 'S';
	mPlayerKey[(int)PlayerKey::Key::Jump] = 'W';
	mPlayerKey[(int)PlayerKey::Key::Reload] = 'R';
	mPlayerKey[(int)PlayerKey::Key::Attack] = VK_LEFT;
	mPlayerKey[(int)PlayerKey::Key::Shift] = VK_SHIFT;
}


PlayerKey::~PlayerKey()
{
	mInputQueue.clear();
}

void PlayerKey::SetupKey(const PlayerKey::Key & key, const int & keyboard)
{
	mPlayerKey[(int)key] = keyboard;
}

const int & PlayerKey::GetPlayerKey(PlayerKey::Key key)
{
	return mPlayerKey[(int)key];
}

bool PlayerKey::GetKeyDown(const Key & key)
{
	if (mInputQueue.size() >= 10)
		mInputQueue.pop_front();

	mInputQueue.push_back(key);
	return _Input->GetKeyDown(mPlayerKey[(int)key]);
}

bool PlayerKey::GetKey(const Key & key)
{
	return _Input->GetKey(mPlayerKey[(int)key]);
}

bool PlayerKey::GetKeyUp(const Key & key)
{
	return _Input->GetKeyUp(mPlayerKey[(int)key]);
}
