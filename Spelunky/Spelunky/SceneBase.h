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
	class ObjectPool*const GetObjectPool()const { return this->mObjectPool; }
	class UpdatePool*const GetUpdatePool()const { return this->mUpdatePool; }
	class RenderPool*const GetRenderPool()const { return this->mRenderPool; }
	class MessageManager*const GetMessagePool()const { return this->mMessagePool; }
	class Physics* const GetPhysics()const { return this->mPhysics; }
	class LightingManager* const GetLightManager()const;
};