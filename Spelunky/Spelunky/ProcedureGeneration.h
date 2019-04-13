#pragma once
class ProcedureGeneration
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
			:prev(nullptr), isLink(false) {}
	};
private:
	int mPass;
	bool mIsStart;
	vector<TileRoom*> mRoomList;
	vector<Figure::FloatRect> mPassageList;
	vector<Vertex*> mVertexList;
	vector<Vector2> mTempVertexList;
	vector<Figure::FloatTriangle> mTriangleList;
	vector<Figure::FloatLine> mLineList;
	vector<Figure::FloatLine> mFinalLineList;
	vector<vector<class Tile*>> mTileList;
	vector<vector<bool>> mCheckList;
	vector<TileRoom*> mSelectList;
	class Looper* mLooper;
	class BackGround* mBackGround;

public:
	ProcedureGeneration();
	~ProcedureGeneration();

	void Update(); 
	void Render();
	void OnGui();
public:
	void AddPass();
	Vertex* FindVertex(const Vector2& pos);
};

