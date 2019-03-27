#pragma once
class UIBase
{
	BlockAssign(UIBase)
protected:
	class Transform* mTransform;
public:
	UIBase(const Vector2& pos, const Vector2& size,
		const Pivot::Enum& pivot, class Transform*const pParent = nullptr);
	virtual ~UIBase();

	class Transform* const GetTransform()const { return mTransform; }

	const bool IsOnMouse();
};

