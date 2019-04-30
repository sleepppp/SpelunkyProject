#include "stdafx.h"
#include "EventManager.h"

#include "InterfaceEvent.h"
EventManager::EventManager()
{
}


EventManager::~EventManager()
{
	for (UINT i = 0; i < mEventQueue.size(); ++i)
		SafeDelete(mEventQueue[i]);
	mEventQueue.clear();
}

void EventManager::Release()
{
	for (UINT i = 0; i < mEventQueue.size(); ++i)
		SafeDelete(mEventQueue[i]);
	mEventQueue.clear();
}
void EventManager::Update()
{
	if (mEventQueue.size() != 0)
	{
		if (mEventQueue[0]->EventUpdate() == true)
		{
			SafeDelete(mEventQueue[0]);
			mEventQueue.erase(mEventQueue.begin());
			if (mEventQueue.size() != 0)
			{
				mEventQueue[0]->EventStart();
			}
		}
	}
}

void EventManager::PushEvent(IEvent * pEvent)
{
	if (mEventQueue.size() == 0)
	{
		pEvent->EventStart();
	}
	this->mEventQueue.push_back(pEvent);
}
