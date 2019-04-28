#include "stdafx.h"
#include "GameSystem.h"

#include "SystemUIController.h"
#include "Transform.h"
#include "RePlayDatas.h"

GameSystem::GameSystem()
	:GameObject("GameSystem"),mCurrentTime(0.f)
{
	mUIController = new SystemUIController;
	mLayer = RenderPool::Layer::UI;
	mState = SystemState::PlayGame;
	mStateQueue.push_back(mState);
}


GameSystem::~GameSystem()
{
	SafeDelete(mUIController);
}

void GameSystem::Init()
{
	_World->GetUpdatePool()->RequestUpdate(this);
	_World->GetRenderPool()->RequestRender(mLayer, this);
}

void GameSystem::Update()
{
	switch (mState)
	{
	case GameSystem::SystemState::PlayGame:
	{
		if (_Input->GetKeyDown(VK_ESCAPE))
		{
			this->ChangeState(SystemState::OpenSystemUI);
		}
	}
		break;
	case GameSystem::SystemState::OpenSystemUI:
		break;
	case GameSystem::SystemState::Continue:
	{
		if (_Input->GetKeyDown(VK_ESCAPE))
		{
			this->ChangeState(SystemState::OpenSystemUI);
		}
		else if (_Input->GetKeyDown(VK_SPACE))
			this->ChangeState(SystemState::Replay);
	}
		break;
	case GameSystem::SystemState::Replay:
	{
		mCurrentTime += _TimeManager->DeltaTime();
		if (mCurrentTime >= 9.f)
		{
			this->OnReplayEnter();
		}

	}
		break;
	default:
		break;
	}
}

void GameSystem::Render()
{
	if (mState == SystemState::Continue)
	{
		_D2DRenderer->RenderTextField(_WinSizeX / 2 - 400, _WinSizeY / 2 - 400, L"요 약",
			80, 800, 300, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"DOSGothic");

		int killCount = _GameData->GetData(GameData::DataType::Int, "KillingMonsterCount")->GetInt();

		_D2DRenderer->RenderTextField(_WinSizeX / 2 - 400, _WinSizeY / 2 - 100, L"잡은 몬스터 수 : " + to_wstring(killCount),
			40, 800, 300, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"DOSGothic");

		_D2DRenderer->RenderTextField(_WinSizeX / 2 - 400, _WinSizeY / 2 + 100, L"죽었습니다! 계속 하려면 Space를 누르세요.",
			35, 800, 300, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"DOSGothic");
	}
	else if (mState == SystemState::Replay)
	{
		_D2DRenderer->RenderTextField(_WinSizeX / 2 - 400, _WinSizeY / 2 - 400, L"리플레이 중 . . . ",
			80, 800, 300, D2DRenderer::DefaultBrush::White, DWRITE_TEXT_ALIGNMENT_CENTER, false, L"DOSGothic");
	}
}

void GameSystem::PlayGame()
{
	mState = SystemState::PlayGame;
	mStateQueue.push_back(mState);
	RePlayManager::Start();

	GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
	groupWorld->GetTransform()->SetActiveAllChild(true);

	if (mStateQueue.size() > 10)
		mStateQueue.pop_front();
}

void GameSystem::OpenSystemUI()
{
	mState = SystemState::OpenSystemUI;
	RePlayManager::Stop();
	mStateQueue.push_back(mState);

	GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
	groupWorld->GetTransform()->SetActiveAllChild(false);

	mUIController->PushUI(_World->GetObjectPool()->FindObject("SystemMainUI"));

	_SoundManager->Play("MenuOpen");
	_SoundManager->Pause(_SoundManager->GetPlayingBGM());

	if (mStateQueue.size() > 10)
	{
		mStateQueue.pop_front();
	}
}

void GameSystem::ChangeState(const SystemState & state)
{
	if (mState == state)
		return;

	mState = state;
	mStateQueue.push_back(mState);
	if (mStateQueue.size() > 10)
		mStateQueue.pop_front();
	if (mState == SystemState::PlayGame)
	{
		//GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
		//groupWorld->GetTransform()->SetActiveAllChild(true);
	}
	else if (mState == SystemState::OpenSystemUI)
	{
		GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
		groupWorld->GetTransform()->SetActiveAllChild(false);

		mUIController->PushUI(_World->GetObjectPool()->FindObject("SystemMainUI"));

		_SoundManager->Play("MenuOpen");
		_SoundManager->Pause(_SoundManager->GetPlayingBGM());
	}
	else if (mState == SystemState::Continue)
	{

	}
	else if (mState == SystemState::Replay)
	{
		this->OnReplayEnter();
	}
}

void GameSystem::ReturnPrevState()
{
	if (mStateQueue.empty())
		return;

	if (mStateQueue.back() == SystemState::OpenSystemUI)
	{
		GameObject* groupWorld = _World->GetObjectPool()->FindObject("World");
		groupWorld->GetTransform()->SetActiveAllChild(true);
	}

	mStateQueue.pop_back();
	SystemState preState = mStateQueue.back();
	mStateQueue.pop_back();

	this->ChangeState(preState);
}

void GameSystem::OnReplayEnter()
{
	const vector<GameObject*>* list = _World->GetObjectPool()->GetObjectList();
	UINT64 current = RePlayManager::GetNowFrame();
	current = current - current % RePlayDatas<int>::RePlayUpdateDelay;
	current = current - RePlayDatas<int>::RePlayUpdateDelay * 7;
	for (UINT i = 0; i < list->size(); ++i)
		list->at(i)->LoadRePlayData(current);
	_TimeManager->Stop();
	_TimeManager->Start();
	mCurrentTime = 0.f;
}
