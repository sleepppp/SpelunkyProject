#pragma once
#include "GameUI.h"
class SystemOptionUI : public GameUI
{
private:
	class Image* mImage;
	float mScale;

	vector<class UIModule*> mButtonList;
public:
	SystemOptionUI();
	virtual ~SystemOptionUI();

	void Init()override;
	void Update()override;
	void Render()override;
private:
	void Enable()override;
	void Disable()override;
};

