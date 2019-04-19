#include "stdafx.h"
#include "Subject.h"

#include "MessageComponent.h"
Subject::Subject()
{
}


Subject::~Subject()
{
	mObserverList.clear();
}

void Subject::Notify(const string & name)
{
	ObserverIter iter = mObserverList.find(name);
	if (iter != mObserverList.end())
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
			iter->second[i]->SendCallbackMessage(name);
	}
}

void Subject::AddObserver(const string & name, MessageComponent * pMessageModule)
{
	mObserverList[name].push_back(pMessageModule);
}

void Subject::RemoveObserver(const string & name, MessageComponent * pMessageModule)
{
	ObserverIter iter = mObserverList.find(name);
	if (iter != mObserverList.end())
	{
		for (UINT i = 0; i < iter->second.size(); ++i)
		{
			if (iter->second[i] == pMessageModule)
			{
				iter->second.erase(iter->second.begin() + i);
				return;
			}
		}
	}
}
