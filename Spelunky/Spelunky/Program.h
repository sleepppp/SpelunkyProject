#pragma once
class Program
{
	BlockAssign(Program)
public:
	Program();
	~Program();

	void Init();
	void Release();
	void Update();
	void Render();
};

