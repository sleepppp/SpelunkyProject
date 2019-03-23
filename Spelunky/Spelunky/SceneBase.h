#pragma once
class SceneBase
{
protected:
	class ObjectPool* mObjectPool;
	class UpdatePool* mUpdatePool; 
	class RenderPool* mRenderPool;
private:
	friend class SceneManager;
	virtual ~SceneBase();
public:
	SceneBase();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
public:
	class ObjectPool*const GetObjectPool()const { return this->mObjectPool; }
	class UpdatePool*const GetUpdatePool()const { return this->mUpdatePool; }
	class RenderPool*const GetRenderPool()const { return this->mRenderPool; }
};

