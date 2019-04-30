#pragma once
class SceneBase
{
	BlockAssign(SceneBase)
protected:
	class ObjectPool* mObjectPool;
	class UpdatePool* mUpdatePool; 
	class RenderPool* mRenderPool;
	class Physics* mPhysics;
	class MessageManager* mMessagePool;
	class EventManager* mEventPool;
protected:
	friend class SceneManager;
	SceneBase();
	virtual ~SceneBase();
public:
	virtual void Init() = 0;
	virtual void PostInit();
	virtual void Release();
	virtual void Update();
	virtual void Render();
public:
	inline class ObjectPool*const GetObjectPool()const { return this->mObjectPool; }
	inline class UpdatePool*const GetUpdatePool()const { return this->mUpdatePool; }
	inline class RenderPool*const GetRenderPool()const { return this->mRenderPool; }
	inline class MessageManager*const GetMessagePool()const { return this->mMessagePool; }
	inline class Physics* const GetPhysics()const { return this->mPhysics; }
	inline class EventManager*const GetEventPool()const { return mEventPool; }
	class LightingManager* const GetLightManager()const;
};