#pragma once
#include "SceneBase.h"
class TestScene :
	public SceneBase
{
private:
	struct TileRoom
	{
		int tileCountX;
		int tileCountY;
		Figure::FloatRect rc;
	};

	struct Node
	{
		Vector2 pos;
		vector<Node*> linkList;
		
	};
public:
	explicit TestScene() {}
	virtual ~TestScene() {}

	void Init()override;
	void Release()override; 
	void Update()override; 
	void Render()override; 

};

