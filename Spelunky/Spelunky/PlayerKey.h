#pragma once
class PlayerKey final
{
	BlockAssign(PlayerKey)
public:
	enum class Key : int
	{
		Left = 0, Right, Jump,Interaction ,Down, Attack, Reload, Shift, End
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

	bool GetKeyDown(const Key& key);
	bool GetKey(const Key& key);
	bool GetKeyUp(const Key& key);

	void PushKey(const Key& key);
	void PopKey(const Key& key);
};

