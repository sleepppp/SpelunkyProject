#pragma once
#include "SceneBase.h"
class TestScene :
	public SceneBase
{
	//BlockAssign(TestScene)
private:
	struct Node
	{
		Figure::FloatRect rc;
		D2D1::ColorF::Enum color;
		vector<Figure::FloatRect*> link;
	};
private:
	int mPass;
	vector<Node> mList;
	float mSpeed;;
	int mRoomCount;

	vector<Vector2> mVertexList;
	vector<Figure::FloatTriangle> mTriangleList;
public:
	explicit TestScene() {}
	virtual ~TestScene() {}

	void Init()override;
	void Release()override; 
	void Update()override; 
	void Render()override; 
private:
	void Reset();
	void NextPass();
};

