#pragma once

class IUpdate
{
	BlockAssign(IUpdate)
private:
	class GameObject* iUpdateObject; 
private:
	friend class GameObject;
	explicit IUpdate(class GameObject* pObject);
	virtual ~IUpdate();
public:
	virtual void Update() = 0;
	inline class GameObject*const GetUpdateObject()const { return iUpdateObject; }
};


class IRender
{
	BlockAssign(IRender)
private:
	class GameObject* iRenderObject;
private:
	friend class GameObject;
	explicit IRender(class GameObject* pObject);
	virtual ~IRender();
public:
	virtual void Render() = 0;
	inline class GameObject*const GetRenderObject()const { return iRenderObject; }
};








