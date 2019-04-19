#pragma once
#include "GameObject.h"
class GameSystem : public GameObject
{
private:
	enum class SystemState
	{
		PlayGame,OpenSystemUI
	};
private:
	SystemState mState;
	class SystemUIController* mUIController;
public:
	GameSystem();
	virtual ~GameSystem();

	void Init()override; 
	void Update()override; 

	void PlayGame();
	void OpenSystemUI();
};

