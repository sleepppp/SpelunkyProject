#pragma once
class MonsterStateBase
{
	BlockAssign(MonsterStateBase)
protected:
	class Monster* mMonster;
	class Player* mPlayer;
public:
	MonsterStateBase(class Monster* pMonster);
	virtual ~MonsterStateBase() {}

	virtual void Enter() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;

	virtual void OnGui() {}

	virtual void OnCollision(const CollideInfo& collisionInfo) {}
};


class MonsterStateManager final
{
private:
	BlockAssign(MonsterStateManager)
private:
	typedef map<string, class MonsterStateBase*>::iterator StateIter;
	typedef map<string, function<void(void)>>::iterator FuncIter;
	map<string, class MonsterStateBase*> mStateList;
	map<string, function<void(void)>> mFuncList;
	class MonsterStateBase* mCurrentState;
public:
	MonsterStateManager();
	virtual ~MonsterStateManager();

	void Update();
	void OnGui();

	void AddState(const string& key, class MonsterStateBase* pState);
	void AddFunc(const string& key, const function<void(void)>& func);
	void ChangeState(const string& key);
	class MonsterStateBase* GetCurrentState()const { return mCurrentState; }
};


class MonsterIdle : public MonsterStateBase
{
public:
	MonsterIdle(class Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class MonsterFlyingToPlayer : public MonsterStateBase
{
private:
	float mAttackDeley;
public:
	MonsterFlyingToPlayer(class Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override; 
	void Execute()override; 
	void Exit()override; 
};

class MonsterShuttling : public MonsterStateBase
{
private:
	class TileManager* mTileManager;
public:
	MonsterShuttling(class Monster* pMonster)
		:MonsterStateBase(pMonster) {}

	void Enter()override;
	void Execute()override;
	void Exit()override;
};