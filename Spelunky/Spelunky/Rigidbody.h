#pragma once
class Rigidbody final
{
	BlockAssign(Rigidbody)
private:
	static const float _defaultJumpPower;
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

	bool mIsOnGround;
	bool mActiveCollision;
	int mMass;
public:
	Rigidbody() = delete;
	explicit Rigidbody(class GameObject* pObject);
	~Rigidbody();

	void Init();
	void Update();

	void Jump(const float& jumpPower = _defaultJumpPower);
	void Move(Vector2 moveValue,const float& speed);
	void Force(const Vector2& direction,const float& power,const float& recuperativePower);

	class GameObject*const GetGameObject()const { return this->mObject; }
	class Transform*const GetTransform()const { return this->mTransform; }
	Figure::FloatRect*const GetLPRect()const;
	const bool& GetActiveCollision()const { return mActiveCollision; }
	const int& GetMass()const { return mMass; }
	void SetMass(const int& mass) { mMass = mass; }
private:
};

