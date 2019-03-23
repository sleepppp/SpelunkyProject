#pragma once

class IUpdate
{
	BlockAssign(IUpdate)
private:
	bool mUpdateActive; 
public:
	IUpdate():mUpdateActive(true) {}
	virtual ~IUpdate() {}

	virtual void Update() = 0;

	const bool& GetUpdateActive()const { return this->mUpdateActive; }
	void SetUpdateActive(bool b) { this->mUpdateActive = b; }
};


class IRender
{
	BlockAssign(IRender)
private:
	bool mRenderActive; 
public:
	IRender() :mRenderActive(true) {}
	virtual ~IRender() {}

	virtual void Render() = 0;

	const bool& GetRenderActive()const { return this->mRenderActive; }
	void SetRenderActive(bool b) { this->mRenderActive = b; }
};








