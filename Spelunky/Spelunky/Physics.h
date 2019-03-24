#pragma once

class Physics final
{
	BlockAssign(Physics)
private:
	friend class SceneBase; 
	Physics();
	~Physics();
	void Release();
	void Update();
};

