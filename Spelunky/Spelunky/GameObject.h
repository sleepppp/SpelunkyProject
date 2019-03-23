#pragma once
class GameObject
{
	BlockAssign(GameObject)
protected:
	string mName; 
	class Transform* mTransform; 
	bool mIsActive; 
public:
	GameObject();
	explicit GameObject(const string& name);
	explicit GameObject(const string& name,const Vector2& pos, const Vector2& size, const Pivot::Enum& pivot);
	virtual ~GameObject();

	virtual void Init() {}
	virtual void Release() {}
public:
	class Transform* const GetTransform()const { return this->mTransform; }
	const string& GetName()const { return this->mName; }
	const bool& GetActive()const { return this->mIsActive; }

	void SetActive(const bool& b);
	void SetName(const string& name) { this->mName = name;  }
private:
	virtual void Enable() {}
	virtual void Disable() {}
};

