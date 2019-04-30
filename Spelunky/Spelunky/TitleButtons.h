#pragma once
#include "GameObject.h"
class TitleButtons : public GameObject
{
	class TextButton* mPlayButton;
	class TextButton* mMapToolButton;
	class TextButton* mGeneratingButton;
	class TextButton* mExitButton;
public:
	TitleButtons();
	virtual ~TitleButtons();
	
	void Init()override; 
	void Release()override;
	void Update()override; 
	void Render()override; 
};

