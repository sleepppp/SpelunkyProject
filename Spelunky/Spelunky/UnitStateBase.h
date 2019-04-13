#pragma once
class UnitStateBase
{
	BlockAssign(UnitStateBase)
protected:
	class Unit* mUnit;
public:
	UnitStateBase(class Unit* pUnit) : mUnit(pUnit) {}
	virtual ~UnitStateBase() {}

	virtual void Enter() = 0;
	virtual void Execute() =0;
	virtual void Exit() = 0;

	virtual void OnCollision(const CollideInfo& info) {}
	virtual void OnTrigger(const CollideInfo& info) {}
};

