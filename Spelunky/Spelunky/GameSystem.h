#pragma once
#include "GameObject.h"
class GameSystem : public GameObject
{
public:
	enum class SystemState
	{
		PlayGame,OpenSystemUI,Continue,Replay
	};
private:
	SystemState mState;
	class SystemUIController* mUIController;
	deque<SystemState> mStateQueue;
	UINT mCurrentTime;
	class Timer* mTimer;
	int mCount;
public:
	GameSystem();
	virtual ~GameSystem();

	void Init()override; 
	void Update()override; 
	void Render()override;

	void PlayGame();
	void OpenSystemUI();
	void ChangeState(const SystemState& state);

	void ReturnPrevState();
	const SystemState& GetSystemState()const { return mState; }
	class SystemUIController* GetUIController()const { return mUIController; }
private:
	void OnReplayEnter();
};

