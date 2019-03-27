#pragma once

class PhysicsTile
{
public:
	vector<class Transform*> transformList;
};

class Physics final
{
	BlockAssign(Physics)
private:
	static float _gravity;
public:
	static const float& GetGravity() { return _gravity; }
private:
	Vector2 mWorldSize; 
	Vector2 mTileSize; 
	int mTileCountX;
	int mTileCountY; 

	vector<vector<PhysicsTile>> mTileList;
	vector<class Transform*> mDynamicList;
private:
	friend class SceneBase; 
	Physics();
	~Physics();
	void Release();
	void Update();
public:

};

