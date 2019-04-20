#include "stdafx.h"
#include "SystemUIController.h"

#include "Transform.h"
#include "GameSystem.h"
SystemUIController::SystemUIController()
{
}


SystemUIController::~SystemUIController()
{
}

void SystemUIController::PushUI(GameObject * pUI)
{
	if (pUI)
	{
		if(mUIStack.empty() == false)
			mUIStack.back()->SetActive(false);

		mUIStack.push_back(pUI);
		pUI->SetActive(true);
	}
}

void SystemUIController::PopUI()
{
	if (mUIStack.empty())
		return; 
	mUIStack.back()->SetActive(false);
	mUIStack.pop_back();
	
	if (mUIStack.empty())
	{
		GameSystem* system = (GameSystem*)_World->GetObjectPool()->FindObject("GameSystem");
		system->PlayGame();
	}
	else
	{
		mUIStack.back()->SetActive(true);
	}
}

bool SystemUIController::IsActiveSystemUI()
{
	return !mUIStack.empty();
}
