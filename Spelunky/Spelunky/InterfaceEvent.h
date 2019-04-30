#pragma once
class IEvent
{
public:
	virtual void EventStart() = 0;
	virtual bool EventUpdate() = 0;
};

class IPlayerDead : public IEvent
{
public:
	void EventStart()override; 
	bool EventUpdate()override;
};