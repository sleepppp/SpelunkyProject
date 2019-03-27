#pragma once
#include "UIBase.h"
class Button : public UIBase
{
	BlockAssign(Button)
protected:
	enum class State : int
	{
		None, Down
	};
protected:
	State mState;
public:
	Button() = delete;
	explicit Button(const Vector2& pos, const Vector2& size,
		const Pivot::Enum& pivot, class Transform*const pParent = nullptr);
	virtual ~Button();

	bool Update(); 
	virtual void Render() = 0; 
};

