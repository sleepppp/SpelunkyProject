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

class IZoomInPlayer : public IEvent
{
public:
	void EventStart()override;
	bool EventUpdate()override; 
};

class IDelay : public IEvent
{
	float iDelayTime;
	float iCurrentTime;
public:
	IDelay(const float& delay) :iDelayTime(delay), iCurrentTime(0.f) {}
	void EventStart()override;
	bool EventUpdate()override;
};

class IZoomOutPlayer : public IEvent
{
public:
	void EventStart()override; 
	bool EventUpdate()override; 
};