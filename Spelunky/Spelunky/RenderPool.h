#pragma once
class RenderPool
{
	BlockAssign(RenderPool)
public:
	enum class Layer : int
	{
		Background = 0,Tile,Object,Effect,UI,End
	};
private:
	typedef unordered_map<Layer, vector<class IRender*>>::iterator RenderIter;
	unordered_map<Layer, vector<class IRender*>> mRenderList;
private:
	friend class SceneBase;
	RenderPool();
	~RenderPool();
	void Release();
	void Render();
public:
	
};

