#pragma once
#include "GameUI.h"
class SystemUIController : public GameObject
{
private:
	deque<GameObject*> mUIStack;
public:
	SystemUIController();
	virtual ~SystemUIController();

	void PushUI(class GameObject* pUI);
	void PopUI();

	bool IsActiveSystemUI();
};

