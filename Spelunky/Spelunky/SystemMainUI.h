#pragma once
#include "GameUI.h"
class SystemMainUI : public GameUI
{
private:
	class Image* mImage;
	float mScale;
public:
	SystemMainUI();
	virtual ~SystemMainUI();

	void Init()override; 
	void Update()override; 
	void Render()override; 

private:
	void Enable()override; 
	void Disable()override; 
};

