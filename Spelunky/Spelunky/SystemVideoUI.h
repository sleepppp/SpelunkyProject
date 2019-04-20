#pragma once
#include "GameUI.h" 
class SystemVideoUI : public GameUI
{
private:
	class Image* mImage;
	float mScale;

	vector<class UIModule*> mButtonList;
public:
	SystemVideoUI();
	virtual ~SystemVideoUI();

	void Init()override;
	void Update()override;
	void Render()override;
private:
	void Enable()override;
	void Disable()override;
};

