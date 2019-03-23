#pragma once
class RenderPool final 
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

	class LightingManager* mLightManager; 

	bool mIsZoder;
private:
	friend class SceneBase;
	RenderPool();
	virtual ~RenderPool();
	void Release();
	void Render();
public:
	void RequestRender(const Layer& layer, class IRender*const pRender);
	void RemoveRender(const Layer& layer, class IRender*const pRender);
	void RemoveRender(const IRender*const pRender);

	void SetZOrder(const bool& b) { mIsZoder = b; }
private:
	void ObjectRender();
	void Lighting();
	void UIRender();
	void ZOrder();
};

