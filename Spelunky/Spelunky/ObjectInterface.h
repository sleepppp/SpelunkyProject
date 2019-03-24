#pragma once

class IUpdate
{
	BlockAssign(IUpdate)
private:
	class GameObject* iUpdateObject; 
private:
	friend class GameObject;
	explicit IUpdate(class GameObject*const pObject);
	virtual ~IUpdate();
private:
	friend class UpdatePool;
	inline class GameObject*const GetUpdateObject()const { return iUpdateObject; }
	virtual void Update() = 0;
};


class IRender
{
	BlockAssign(IRender)
private:
	class GameObject* iRenderObject;
private:
	friend class GameObject;
	explicit IRender(class GameObject*const pObject);
	virtual ~IRender();
private:
	friend class RenderPool;
	inline class GameObject*const GetRenderObject()const { return iRenderObject; }
	virtual void Render() = 0;
};








