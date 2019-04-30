#pragma once
class EventManager final
{
	BlockAssign(EventManager)
private:
	deque<class IEvent*> mEventQueue;
public:
	EventManager();
	~EventManager();

	void Release();
	void Update();

	void PushEvent(class IEvent* pEvent);
};

