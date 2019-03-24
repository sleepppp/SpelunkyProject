#pragma once
class SceneBase
{
	BlockAssign(SceneBase)
protected:
	class ObjectPool* mObjectPool;
	class UpdatePool* mUpdatePool; 
	class RenderPool* mRenderPool;
	class Physics* mPhysics;
protected:
	friend class SceneManager;
	SceneBase();
	virtual ~SceneBase();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
public:
	class ObjectPool*const GetObjectPool()const { return this->mObjectPool; }
	class UpdatePool*const GetUpdatePool()const { return this->mUpdatePool; }
	class RenderPool*const GetRenderPool()const { return this->mRenderPool; }
	class Physics* const GetPhysics()const { return this->mPhysics; }
	class LightingManager* const GetLightManager()const;
};