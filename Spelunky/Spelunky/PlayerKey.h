#pragma once
class PlayerKey final
{
	BlockAssign(PlayerKey)
public:
	enum class Key : int
	{
		Left = 0, Right, Jump,Interaction ,Down, Attack, Bomb, Shift, End
	};
private:
	enum class KeyState : int
	{
		None = 0,Down,Stay,Up,End
	};
private:
	int mPlayerKey[(int)Key::End];
	KeyState mPlayerKeyState[(int)Key::End];
public:
	PlayerKey();
	~PlayerKey();

	void Update();

	void CheckPreKeyState();

	void SetupKey(const PlayerKey::Key& key, const int& keyboard);
	const int& GetPlayerKey(PlayerKey::Key key);

	inline bool GetKeyDown(const Key& key)
	{
		if (IsMouseOnGui == true)return false;
		if (mPlayerKeyState[(int)key] == KeyState::Down)
			return true;
		return false;
	 }
	inline bool GetKey(const Key& key)
	{
		if (IsMouseOnGui == true)return false;
		if (mPlayerKeyState[(int)key] == KeyState::Stay ||
			mPlayerKeyState[(int)key] == KeyState::Down)
			return true;
		return false;
	 }
	inline bool GetKeyUp(const Key& key)
	{
		if (IsMouseOnGui == true)return false;
		if (mPlayerKeyState[(int)key] == KeyState::Up)
			return true;
		return false;
	}

	void PushKey(const Key& key);
	void PopKey(const Key& key);
};

