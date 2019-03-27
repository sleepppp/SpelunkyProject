#pragma once
class GameObject
{
	BlockAssign(GameObject)
protected:
	string mName; 
	class Transform* mTransform; 
	bool mIsActive; 
	RenderPool::Layer mLayer;
public:
	GameObject();
	explicit GameObject(const string& name);
	explicit GameObject(const string& name,const Vector2& pos, const Vector2& size, 
		const Pivot::Enum& pivot,const RenderPool::Layer& layer = RenderPool::Layer::Object);
	virtual ~GameObject();

	virtual void Init() {}
	virtual void Release() {}
	virtual void Update() {}
	virtual void Render() {}
public:
	inline class Transform* const GetTransform()const { return this->mTransform; }
	const string& GetName()const { return this->mName; }
	const bool& GetActive()const { return this->mIsActive; }
	const RenderPool::Layer& GetLayer()const { return this->mLayer; }

	void SetActive(const bool& b);
	void SetName(const string& name) { this->mName = name;  }
private:
	virtual void Enable() {}
	virtual void Disable() {}
};

