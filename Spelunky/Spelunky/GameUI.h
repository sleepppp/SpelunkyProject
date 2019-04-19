#pragma once
#include "GameObject.h"
class GameUI : public GameObject
{
protected:
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
};

