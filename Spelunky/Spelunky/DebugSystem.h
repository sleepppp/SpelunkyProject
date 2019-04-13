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
	bool mIsOpenCameraGui;
	bool mIsOpenLightGui;
public:
	void Render();
};

