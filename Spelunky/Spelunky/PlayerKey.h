#pragma once
#include <deque>
class PlayerKey final
{
	BlockAssign(PlayerKey)
public:
	enum class Key : int
	{
		Left = 0, Right, Jump, Down, Attack, Reload, Shift, End
	};
private:
	int mPlayerKey[(int)Key::End];
	deque<Key> mInputQueue;
public:
	PlayerKey();
	~PlayerKey();

	void SetupKey(const PlayerKey::Key& key, const int& keyboard);
	const int& GetPlayerKey(PlayerKey::Key key);

	bool GetKeyDown(const Key& key);
	bool GetKey(const Key& key);
	bool GetKeyUp(const Key& key);
};

