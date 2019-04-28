#pragma once
class UnitStateManager final 
{
	BlockAssign(UnitStateManager)
private:
	typedef map<string, class UnitStateBase*>::iterator StateIter;
	map<string, class UnitStateBase*> mStateList;
	class UnitStateBase* mCurrentState;
	string mCurrentKey;
public:
	UnitStateManager();
	virtual ~UnitStateManager();

	void Update();
	void OnGui();

	void AddState(const string& key, class UnitStateBase* pState);
	void ChangeState(const string& key);
	class UnitStateBase* GetCurrentState()const { return mCurrentState; }
	string GetCurrentKey()const { return mCurrentKey; }
};

