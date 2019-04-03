#pragma once
#include "SceneBase.h"
#include <set>
class DelaunayScene : public SceneBase
{
private:
	struct TileRoom
	{
		int tileCountX; 
		int tileCountY;
		Figure::FloatRect rc;
		bool isSelect;
	};
	struct Vertex
	{
		Vector2 pos;
		set<Vertex*> link;
		typedef set<Vertex*>::iterator LinkIter;
	};
private:
	int mPass; 
	vector<TileRoom*> mRoomList;
	vector<Vertex> mVertexList;
	vector<Figure::FloatTriangle> mTriangleList;
	vector<Figure::FloatLine> mLineList;
	vector<Figure::FloatLine> mFinalLineList;
public:
	DelaunayScene();
	virtual ~DelaunayScene();

	void Init()override;
	void Release()override; 
	void Update()override;
	void Render()override; 
private:
	void NextPass();
	void Reset();
};

