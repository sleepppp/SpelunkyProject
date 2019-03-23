#pragma once
class GameObject
{
protected:
	class Transform* mTransform; 
	bool mIsActive; 
public:
	GameObject();
	virtual ~GameObject();

	virtual void Init() {}
	virtual void Release() {}

public:
	void SetActive(const bool& b);
private:
	virtual void Enable() {}
	virtual void Disable() {}
};

