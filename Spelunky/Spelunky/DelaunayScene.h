#pragma once
#include "SceneBase.h"
class DelaunayScene : public SceneBase
{
private:
	vector<Vector2> mVertexList;

	Figure::FloatTriangle mTriangle;
	Vector2 mCenter; 
	float mRadius;
public:
	DelaunayScene();
	virtual ~DelaunayScene();

	void Init()override;
	void Release()override; 
	void Update()override;
	void Render()override; 
};

