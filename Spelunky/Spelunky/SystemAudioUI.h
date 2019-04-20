#pragma once
#include "GameUI.h"
class SystemAudioUI : public GameUI
{
private:
	class Image* mImage;
	float mScale;

	vector<class UIModule*> mButtonList;
public:
	SystemAudioUI();
	virtual ~SystemAudioUI();

	void Init()override;
	void Update()override;
	void Render()override;
private:
	void Enable()override;
	void Disable()override;
};

