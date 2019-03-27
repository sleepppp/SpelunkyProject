#pragma once

class ObjectPool final
{
	BlockAssign(ObjectPool)
private:
	vector<class GameObject*> mObjectList;
private:
	friend class SceneBase;
	ObjectPool();
	virtual ~ObjectPool();
	void Init();
	void Release();
public:
	void AddObject(class GameObject*const pObject);
	void DeleteObject(class GameObject*const pObject);
	
	class GameObject*const FindObject(const string& name);
	vector<class GameObject*> FindObjects(const string& name);
	const vector<class GameObject*>*const GetObjectList();
};

