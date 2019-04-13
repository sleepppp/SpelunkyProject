#pragma once

class ICollision
{
public:
	virtual void OnCollision(const CollideInfo& info) = 0;
};

class ITrigger
{
public:
	virtual void OnTrigger(const CollideInfo& info) = 0;
};