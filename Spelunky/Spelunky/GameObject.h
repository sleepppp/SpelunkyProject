#pragma once
#include "MessageComponent.h"
class GameObject : public MessageComponent
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
	virtual void OnGui() {}
public:
	inline class Transform* const GetTransform()const { return this->mTransform; }
	const string& GetName()const { return this->mName; }
	const bool& GetActive()const { return this->mIsActive; }
	const RenderPool::Layer& GetLayer()const { return this->mLayer; }
	virtual Figure::FloatRect GetCollisionRect();
	void SetActive(const bool& b);
	void SetName(const string& name) { this->mName = name;  }
protected:
	virtual void Enable() { this->mIsActive = true; }
	virtual void Disable() { this->mIsActive = false; }
};

