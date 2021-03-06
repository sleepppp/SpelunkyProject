#pragma once
class Rigidbody final
{
private:
	static const float _defaultJumpPower;
	static const float _defaultRecuperativePower;
public:
	static float GetDefaultJumpPower();
private:
	enum class CollisionState
	{
		None,Enter,Stay,Exit
	};
private:
	class TileManager* mTileManager;
	class GameObject* mObject; 
	class Transform* mTransform; 

	float mJumpPower;

	Vector2 mForceDirection;
	float mForcePower; 
	float mRecuperativePower; 
	int mCheckRange;

	bool mIsOnGround;
	bool mIsActiveGravity;
public:
	Rigidbody();
	explicit Rigidbody(class GameObject* pObject);
	~Rigidbody();

	void Init();
	void Update();

	void Jump(const float& jumpPower = _defaultJumpPower);
	void Move(Vector2 moveValue,const float& speed);
	void Force(const Vector2& direction,const float& power,const float& recuperativePower = _defaultRecuperativePower);
	void ActiveGravity() { mIsActiveGravity = true; }
	void DisActiveGravity() { mIsActiveGravity = false; }
	void ZeroForce();
	void SetCheckRange(const int& range) { mCheckRange = range; }

	class GameObject*const GetGameObject()const { return this->mObject; }
	class Transform*const GetTransform()const { return this->mTransform; }
	Figure::FloatRect*const GetLPRect()const;
	float GetJumpPower()const { return mJumpPower; }
};

