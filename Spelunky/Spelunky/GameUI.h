#pragma once
#include "GameObject.h"
class GameUI : public GameObject
{
public:
	enum class State : int
	{
		Active,Activation,DeActivation,End
	};
protected:
	class SystemUIController* mController;
	State mState;
public:
	GameUI();
	virtual ~GameUI();

	virtual void Init()override; 

	void ChangeState(const GameUI::State& state) { mState = state; };
	class SystemUIController* GetController()const { return mController; }
};

