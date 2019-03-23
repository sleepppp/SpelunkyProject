#pragma once
class UpdatePool
{
	BlockAssign(UpdatePool)
private:
	vector<class IUpdate*> mUpdateList;
private:
	friend class SceneBase;
	UpdatePool();
	virtual ~UpdatePool();
	void Update();
	void Release();
public:
	void RequestUpdate(class IUpdate*const pUpdate);
	void RemoveUpdate(const class IUpdate*const pUpdate);
};

