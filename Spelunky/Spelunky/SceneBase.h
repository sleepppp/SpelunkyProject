#pragma once
class SceneBase
{
public:
	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};
