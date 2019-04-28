#pragma once
#include "GameObject.h"
#include "RePlayDatas.h"
class Item : public GameObject
{
protected:
	static const Vector2 _itemSize;
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

	virtual void EnterInstallation() = 0;
	virtual void Execute() = 0;
	virtual void ExitInstallation() = 0;

	void Installing(class Unit* pUnit);
	void UnInstallation();
	class Rigidbody* GetRigidBody()const { return mRigidbody; }

};

