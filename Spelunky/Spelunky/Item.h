#pragma once
#include "GameObject.h"
class Item : public GameObject
{
private:
	static const Vector2 _itemSize;
	static const float _installationRange;
protected:
	ImageInfo mImageInfo;
	bool mIsInstallation;
	class Rigidbody* mRigidbody;
	class Unit* mUnit;
	float mAngle;
public:
	Item(const Vector2& worldPos,const bool& mIsInstallation = false);
	virtual ~Item();
	
	virtual void Init()override;
	virtual void Release()override; 
	virtual void Update()override;
	virtual void Render()override;

	virtual void Enter() = 0;
	virtual void Execute() = 0;
	virtual void Exit() = 0;

	void Installing(class Unit* pUnit);
	void UnInstallation();
	class Rigidbody* GetRigidBody()const { return mRigidbody; }
};

