#include "stdafx.h"
#include "DebugSystem.h"


DebugSystem::DebugSystem()
	:mIsOpenCameraGui(false)
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
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();

		_TimeManager->Render();
	}
}
