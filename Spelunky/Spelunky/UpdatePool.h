#pragma once
class UpdatePool final
{
	BlockAssign(UpdatePool)
private:
	vector<class GameObject*> mUpdateList;
private:
	friend class SceneBase;
	UpdatePool();
	virtual ~UpdatePool();
	void Update();
	void Release();
public:
	void RequestUpdate(class GameObject*const pUpdate);
	void RemoveUpdate(const class GameObject*const pUpdate);
};

