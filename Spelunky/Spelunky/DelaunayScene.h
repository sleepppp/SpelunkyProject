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
		vector<Vertex*> link;
		Vertex* prev;
		bool isLink;
		typedef vector<Vertex*>::iterator LinkIter;
		Vertex()
			:prev(nullptr),isLink(false) {}
	};
private:
	int mPass;
	vector<TileRoom*> mRoomList;
	vector<Figure::FloatRect> mPassageList;
	vector<Vertex*> mVertexList;
	vector<Vector2> mTempVertexList;
	vector<Figure::FloatTriangle> mTriangleList;
	vector<Figure::FloatLine> mLineList;
	vector<Figure::FloatLine> mFinalLineList;
	vector<vector<class Tile*>> mTileList;
	vector<class GameObject*> mObjectList;
	class BackGround* mBackGround;
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
	Vertex* FindVertex(const Vector2& pos);
	
};

