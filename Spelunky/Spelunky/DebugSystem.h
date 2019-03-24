#pragma once
#include "SingletonBase.h"
class DebugSystem final : public SingletonBase<DebugSystem> 
{
	BlockAssign(DebugSystem)
private:
	friend class SingletonBase<DebugSystem>;
	DebugSystem();
	virtual ~DebugSystem();
private:
	bool mIsDebug;

public:
	inline const bool& GetDebugMode()const { return this->mIsDebug; }
	void SetDebugMode(const bool& b) { this->mIsDebug; }
};

