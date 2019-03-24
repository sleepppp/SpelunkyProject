#pragma once

class IOnTriggerEnter
{
	BlockAssign(IOnTriggerEnter)
public:
	virtual void OnTriggerEnter(const class Bounding*const pBound) = 0; 
};

class IOnTriggerStay
{
	BlockAssign(IOnTriggerStay)
public:
	virtual void OnTriggerStay(const class Bounding* const pBound) = 0;
};

class IOnTriggerExit
{
	BlockAssign(IOnTriggerExit)
public:
	virtual void OnTriggerExit(const class Bounding* const pBound) = 0;
};

class IOnCollisionEnter
{
	BlockAssign(IOnCollisionEnter)
public:
	virtual void OnCollisionEnter(const class Bounding* const pBound) = 0;
};

class IOnCollisionStay
{
	BlockAssign(IOnCollisionStay)
public:
	virtual void OnCollisionStay(const class Bounding* const pBound) = 0;
};

class IOnCollisionExit
{
	BlockAssign(IOnCollisionExit)
public:
	virtual void OnCollisionExit(const class Bounding* const pBound) = 0;
};
