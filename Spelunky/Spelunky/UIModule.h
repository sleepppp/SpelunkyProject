#pragma once
class UIModule
{
protected:
	enum class State
	{
		None,OnMouse,Click
	};
protected:
	class Transform* mTransform;
	State mState;
public:
	UIModule(const Vector2& pos, const Vector2& size);
	virtual ~UIModule();

	virtual void Update() = 0;
	virtual void Render() = 0;
};

