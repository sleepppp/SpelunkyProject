#include "stdafx.h"
#include "DebugSystem.h"

#include "LightingManager.h"
DebugSystem::DebugSystem()
	:mIsOpenCameraGui(false), mIsOpenLightGui(false)
{
}


DebugSystem::~DebugSystem()
{
}

void DebugSystem::Render()
{
	if (_isDebug)
	{
		ImGui::BeginMainMenuBar();
		{
			if (ImGui::BeginMenu("System"))
			{
				if (ImGui::MenuItem("Camera"))
					this->mIsOpenCameraGui = !mIsOpenCameraGui;
				if (ImGui::MenuItem("Light"))
					mIsOpenLightGui = !mIsOpenLightGui;
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		_TimeManager->Render();
		if (mIsOpenCameraGui)
			_Camera->OnGui();
		if (mIsOpenLightGui)
			_World->GetLightManager()->OnGui();
	}
}
