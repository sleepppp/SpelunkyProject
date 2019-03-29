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
	
private:
	friend class SceneBase; 
	Physics();
	~Physics();
	void Release();
	void Update();
public:
	bool IntersectPhysics(Figure::FloatRect*const rc1, Figure::FloatRect*const rc2, const float& speed);
};

